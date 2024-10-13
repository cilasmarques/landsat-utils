#include "endmembers.h"

void get_quartiles(float *target, float *v_quartile, int height_band, int width_band, float first_interval, float middle_interval, float last_interval)
{
  const int SIZE = height_band * width_band;
  float *target_values = (float *)malloc(sizeof(float) * SIZE);

  if (target_values == NULL)
    exit(15);

  int pos = 0;
  for (int i = 0; i < height_band * width_band; i++)
  {
    if (!isnan(target[i]) && !isinf(target[i]))
    {
      target_values[pos] = target[i];
      pos++;
    }
  }

  int first_index = static_cast<int>(floor(first_interval * pos));
  int middle_index = static_cast<int>(floor(middle_interval * pos));
  int last_index = static_cast<int>(floor(last_interval * pos));

  std::nth_element(target_values, target_values + first_index, target_values + pos);
  v_quartile[0] = target_values[first_index];

  std::nth_element(target_values, target_values + middle_index, target_values + pos);
  v_quartile[1] = target_values[middle_index];

  std::nth_element(target_values, target_values + last_index, target_values + pos);
  v_quartile[2] = target_values[last_index];

  free(target_values);
}

pair<Candidate, Candidate> getEndmembers(float *ndvi, float *surface_temperature, float *albedo, float *net_radiation, float *soil_heat, int height_band, int width_band, int height_limit, int width_limit)
{
  vector<Candidate> hotCandidates;
  vector<Candidate> coldCandidates;

  vector<float> tsQuartile(3);
  vector<float> ndviQuartile(3);
  vector<float> albedoQuartile(3);

  // STEEP
  get_quartiles(ndvi, ndviQuartile.data(), height_band, width_band, 0.15, 0.97, 0.97);
  get_quartiles(albedo, albedoQuartile.data(), height_band, width_band, 0.25, 0.50, 0.75);
  get_quartiles(surface_temperature, tsQuartile.data(), height_band, width_band, 0.20, 0.85, 0.97);

  // SEBAL
  // get_quartiles(ndvi, ndviQuartile.data(), height_band, width_band, 0.25, 0.75, 0.75);
  // get_quartiles(albedo, albedoQuartile.data(), height_band, width_band, 0.25, 0.50, 0.75);
  // get_quartiles(surface_temperature, tsQuartile.data(), height_band, width_band, 0.25, 0.75, 0.75);

  float *ho = (float *)malloc(sizeof(float) * height_band * width_band);
  for (int i = 0; i < height_band * width_band; i++)
  {
    int line = i / width_band;
    int col = i % width_band;

    ho[i] = net_radiation[i] - soil_heat[i];

    bool hotNDVI = !std::isnan(ndvi[i]) && ndvi[i] > 0.10 && ndvi[i] < ndviQuartile[0];
    bool hotAlbedo = !std::isnan(albedo[i]) && albedo[i] > albedoQuartile[1] && albedo[i] < albedoQuartile[2];
    bool hotTS = !std::isnan(surface_temperature[i]) && surface_temperature[i] > tsQuartile[1] && surface_temperature[i] < tsQuartile[2];

    bool coldNDVI = !std::isnan(ndvi[i]) && ndvi[i] > ndviQuartile[2];
    bool coldAlbedo = !std::isnan(surface_temperature[i]) && albedo[i] > albedoQuartile[0] && albedo[i] < albedoQuartile[1];
    bool coldTS = !std::isnan(albedo[i]) && surface_temperature[i] < tsQuartile[0];

    if (hotAlbedo && hotNDVI && hotTS)
      hotCandidates.emplace_back(ndvi[i], surface_temperature[i], net_radiation[i], soil_heat[i], ho[i], line, col);

    if (coldNDVI && coldAlbedo && coldTS)
      coldCandidates.emplace_back(ndvi[i], surface_temperature[i], net_radiation[i], soil_heat[i], ho[i], line, col);
  }

  if (hotCandidates.empty() || coldCandidates.empty())
  {
    cerr << "Pixel problem! - There are no final candidates";
    exit(15);
  }

  std::sort(hotCandidates.begin(), hotCandidates.end(), compare_candidate_temperature);
  std::sort(coldCandidates.begin(), coldCandidates.end(), compare_candidate_temperature);

  float *ndvi_aux, *ts_aux, *albedo_aux, *net_radiation_aux, *soil_heat_aux;
  ndvi_aux = (float *)malloc(sizeof(float) * height_limit * width_limit);
  ts_aux = (float *)malloc(sizeof(float) * height_limit * width_limit);
  albedo_aux = (float *)malloc(sizeof(float) * height_limit * width_limit);
  net_radiation_aux = (float *)malloc(sizeof(float) * height_limit * width_limit);
  soil_heat_aux = (float *)malloc(sizeof(float) * height_limit * width_limit);

  pair<Candidate, Candidate> pixels;
  for (int i = 0; i < hotCandidates.size(); i++)
  {
    for (int j = 0; j < coldCandidates.size(); j++)
    {
      if (abs(hotCandidates[i].line - coldCandidates[j].line) < height_limit && abs(hotCandidates[i].col - coldCandidates[j].col) < width_limit)
      {
        int initial_line = hotCandidates[i].line < coldCandidates[j].line ? hotCandidates[i].line : coldCandidates[j].line;
        int initial_col = hotCandidates[i].col < coldCandidates[j].col ? hotCandidates[i].col : coldCandidates[j].col;
        int final_line = hotCandidates[i].line > coldCandidates[j].line ? hotCandidates[i].line : coldCandidates[j].line;
        int final_col = hotCandidates[i].col > coldCandidates[j].col ? hotCandidates[i].col : coldCandidates[j].col;

        for (int k = initial_line; k < final_line; k++)
        {
          for (int l = initial_col; l < final_col; l++)
          {
            ndvi_aux[(k - initial_line) * width_limit + (l - initial_col)] = ndvi[k * width_band + l];
            ts_aux[(k - initial_line) * width_limit + (l - initial_col)] = surface_temperature[k * width_band + l];
            albedo_aux[(k - initial_line) * width_limit + (l - initial_col)] = albedo[k * width_band + l];
            net_radiation_aux[(k - initial_line) * width_limit + (l - initial_col)] = net_radiation[k * width_band + l];
            soil_heat_aux[(k - initial_line) * width_limit + (l - initial_col)] = soil_heat[k * width_band + l];
          }
        }

        try
        {
          pixels = endmembersSeconfFilter(ndvi_aux, ts_aux, albedo_aux, net_radiation_aux, soil_heat_aux, height_limit, width_limit, height_limit, width_limit);
          std::cout << "Found" << std::endl;
          return {hotCandidates[i], coldCandidates[j]};
        }
        catch (const std::exception &e)
        {
          // do nothing and continue
        }
      }
    }
  }

  return pixels;
}

pair<Candidate, Candidate> endmembersSeconfFilter(float *ndvi, float *surface_temperature, float *albedo, float *net_radiation, float *soil_heat, int height_band, int width_band, int height_limit, int width_limit)
{
  vector<Candidate> hotCandidates;
  vector<Candidate> coldCandidates;

  vector<float> tsQuartile(3);
  vector<float> ndviQuartile(3);
  vector<float> albedoQuartile(3);
  get_quartiles(ndvi, ndviQuartile.data(), height_band, width_band, 0.15, 0.97, 0.97);
  get_quartiles(albedo, albedoQuartile.data(), height_band, width_band, 0.25, 0.50, 0.75);
  get_quartiles(surface_temperature, tsQuartile.data(), height_band, width_band, 0.20, 0.85, 0.97);

  float *ho = (float *)malloc(sizeof(float) * height_band * width_band);
  for (int i = 0; i < height_band * width_band; i++)
  {
    ho[i] = net_radiation[i] - soil_heat[i];

    bool hotAlbedo = !std::isnan(albedo[i]) && albedo[i] > albedoQuartile[1];
    bool hotNDVI = !std::isnan(ndvi[i]) && ndvi[i] > 0.10 && ndvi[i] < ndviQuartile[0];
    bool hotTS = !std::isnan(surface_temperature[i]) && surface_temperature[i] > tsQuartile[1];

    bool coldAlbedo = !std::isnan(albedo[i]) && albedo[i] < albedoQuartile[1];
    bool coldNDVI = !std::isnan(ndvi[i]) && ndvi[i] >= ndviQuartile[1];
    bool coldTS = !std::isnan(surface_temperature[i]) && surface_temperature[i] < tsQuartile[0];

    int line = i / width_band;
    int col = i % width_band;

    if (hotAlbedo && hotNDVI && hotTS)
      hotCandidates.emplace_back(ndvi[i], surface_temperature[i], net_radiation[i], soil_heat[i], ho[i], line, col);

    if (coldNDVI && coldAlbedo && coldTS)
      coldCandidates.emplace_back(ndvi[i], surface_temperature[i], net_radiation[i], soil_heat[i], ho[i], line, col);
  }

  if (hotCandidates.empty() || coldCandidates.empty())
  {
    throw std::runtime_error("Pixel problem! - There are no final candidates");
  }

  std::sort(hotCandidates.begin(), hotCandidates.end(), compare_candidate_temperature);
  std::sort(coldCandidates.begin(), coldCandidates.end(), compare_candidate_temperature);

  unsigned int hotPos = static_cast<unsigned int>(std::floor(hotCandidates.size() * 0.5));
  unsigned int coldPos = static_cast<unsigned int>(std::floor(coldCandidates.size() * 0.5));

  return {hotCandidates[hotPos], coldCandidates[coldPos]};
}
