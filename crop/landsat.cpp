#include "landsat.h"

Landsat::Landsat(string bands_paths[], MTL mtl)
{
  this->mtl = mtl;

  // Load the bands
  this->bands_resampled[0] = TIFFOpen(bands_paths[0].c_str(), "r");
  this->bands_resampled[1] = TIFFOpen(bands_paths[1].c_str(), "r");
  this->bands_resampled[2] = TIFFOpen(bands_paths[2].c_str(), "r");
  this->bands_resampled[3] = TIFFOpen(bands_paths[3].c_str(), "r");
  this->bands_resampled[4] = TIFFOpen(bands_paths[4].c_str(), "r");
  this->bands_resampled[5] = TIFFOpen(bands_paths[5].c_str(), "r");
  this->bands_resampled[6] = TIFFOpen(bands_paths[6].c_str(), "r");
  this->bands_resampled[7] = TIFFOpen(bands_paths[7].c_str(), "r");

  // Get the dimensions
  TIFFGetField(this->bands_resampled[0], TIFFTAG_IMAGEWIDTH, &this->width_band);
  TIFFGetField(this->bands_resampled[0], TIFFTAG_IMAGELENGTH, &this->height_band);

  this->products = Products(this->width_band, this->height_band);

  // Get bands metadata
  uint16_t sample_format;
  TIFFGetField(this->bands_resampled[1], TIFFTAG_SAMPLEFORMAT, &sample_format);

  this->sample_bands = sample_format;

  // Get bands data
  for (int i = 0; i < 7; i++)
  {
    for (int line = 0; line < this->height_band; line++)
    {
      TIFF *curr_band = this->bands_resampled[i];
      tdata_t band_line_buff = _TIFFmalloc(TIFFScanlineSize(curr_band));
      unsigned short curr_band_line_size = TIFFScanlineSize(curr_band) / this->width_band;
      TIFFReadScanline(curr_band, band_line_buff, line);

      for (int col = 0; col < this->width_band; col++)
      {
        float value = 0;
        memcpy(&value, static_cast<unsigned char *>(band_line_buff) + col * curr_band_line_size, curr_band_line_size);

        switch (i)
        {
        case 0:
          this->products.band_blue[line * this->width_band + col] = value;
          break;
        case 1:
          this->products.band_green[line * this->width_band + col] = value;
          break;
        case 2:
          this->products.band_red[line * this->width_band + col] = value;
          break;
        case 3:
          this->products.band_nir[line * this->width_band + col] = value;
          break;
        case 4:
          this->products.band_swir1[line * this->width_band + col] = value;
          break;
        case 5:
          this->products.band_termal[line * this->width_band + col] = value;
          break;
        case 6:
          this->products.band_swir2[line * this->width_band + col] = value;
          break;
        default:
          break;
        }
      }
      _TIFFfree(band_line_buff);
    }
  }

  // Get tal data
  TIFF *elevation_band = this->bands_resampled[7];
  for (int line = 0; line < this->height_band; line++)
  {
    tdata_t band_line_buff = _TIFFmalloc(TIFFScanlineSize(elevation_band));
    unsigned short curr_band_line_size = TIFFScanlineSize(elevation_band) / this->width_band;
    TIFFReadScanline(elevation_band, band_line_buff, line);

    for (int col = 0; col < this->width_band; col++)
    {
      float value = 0;
      memcpy(&value, static_cast<unsigned char *>(band_line_buff) + col * curr_band_line_size, curr_band_line_size);

      this->products.elevation[line * this->width_band + col] = value;
      this->products.tal[line * this->width_band + col] = 0.75 + 2 * pow(10, -5) * value;
    }
    _TIFFfree(band_line_buff);
  }
};

string Landsat::compute_Rn_G(Station station)
{
  string result = "";
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  result += products.radiance_function(mtl);
  result += products.reflectance_function(mtl);
  result += products.albedo_function(mtl);

  // Vegetation indices
  result += products.ndvi_function();
  result += products.pai_function();
  result += products.lai_function();
  result += products.evi_function();

  // Emissivity indices
  result += products.enb_emissivity_function();
  result += products.eo_emissivity_function();
  result += products.ea_emissivity_function();
  result += products.surface_temperature_function(mtl);

  // Radiation waves
  result += products.short_wave_radiation_function(mtl);
  result += products.large_wave_radiation_surface_function();
  result += products.large_wave_radiation_atmosphere_function(station.temperature_image);

  // Main products
  result += products.net_radiation_function();
  result += products.soil_heat_flux_function();

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  result += "SERIAL,P1_INITIAL_PROD," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
  return result;
}

string Landsat::select_endmembers(int method, int height_limit, int width_limit)
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  pair<Candidate, Candidate> pixels = getEndmembers(products.ndvi, products.surface_temperature, products.albedo, products.net_radiation, products.soil_heat, this->height_band, this->width_band, height_limit, width_limit);
  hot_pixel = pixels.first;
  cold_pixel = pixels.second;

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  return "SERIAL,P2_PIXEL_SEL," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
}

void Landsat::close()
{
  for (int i = 0; i < 8; i++)
  {
    TIFFClose(this->bands_resampled[i]);
  }
};