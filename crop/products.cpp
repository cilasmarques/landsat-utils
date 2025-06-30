#include "products.h"

Products::Products() {}

Products::Products(uint32_t width_band, uint32_t height_band)
{
  this->width_band = width_band;
  this->height_band = height_band;
  this->nBytes_band = height_band * width_band * sizeof(float);

  this->band_blue = (float *)malloc(nBytes_band);
  this->band_green = (float *)malloc(nBytes_band);
  this->band_red = (float *)malloc(nBytes_band);
  this->band_nir = (float *)malloc(nBytes_band);
  this->band_swir1 = (float *)malloc(nBytes_band);
  this->band_termal = (float *)malloc(nBytes_band);
  this->band_swir2 = (float *)malloc(nBytes_band);
  this->tal = (float *)malloc(nBytes_band);
  this->elevation = (float *)malloc(nBytes_band);

  this->radiance_blue = (float *)malloc(nBytes_band);
  this->radiance_green = (float *)malloc(nBytes_band);
  this->radiance_red = (float *)malloc(nBytes_band);
  this->radiance_nir = (float *)malloc(nBytes_band);
  this->radiance_swir1 = (float *)malloc(nBytes_band);
  this->radiance_termal = (float *)malloc(nBytes_band);
  this->radiance_swir2 = (float *)malloc(nBytes_band);

  this->reflectance_blue = (float *)malloc(nBytes_band);
  this->reflectance_green = (float *)malloc(nBytes_band);
  this->reflectance_red = (float *)malloc(nBytes_band);
  this->reflectance_nir = (float *)malloc(nBytes_band);
  this->reflectance_swir1 = (float *)malloc(nBytes_band);
  this->reflectance_termal = (float *)malloc(nBytes_band);
  this->reflectance_swir2 = (float *)malloc(nBytes_band);

  this->albedo = (float *)malloc(nBytes_band);
  this->ndvi = (float *)malloc(nBytes_band);
  this->soil_heat = (float *)malloc(nBytes_band);
  this->surface_temperature = (float *)malloc(nBytes_band);
  this->net_radiation = (float *)malloc(nBytes_band);
  this->lai = (float *)malloc(nBytes_band);
  this->savi = (float *)malloc(nBytes_band);
  this->evi = (float *)malloc(nBytes_band);
  this->pai = (float *)malloc(nBytes_band);
  this->enb_emissivity = (float *)malloc(nBytes_band);
  this->eo_emissivity = (float *)malloc(nBytes_band);
  this->ea_emissivity = (float *)malloc(nBytes_band);
  this->short_wave_radiation = (float *)malloc(nBytes_band);
  this->large_wave_radiation_surface = (float *)malloc(nBytes_band);
  this->large_wave_radiation_atmosphere = (float *)malloc(nBytes_band);
  this->surface_temperature = (float *)malloc(nBytes_band);
};

void Products::close()
{
  free(this->band_blue);
  free(this->band_green);
  free(this->band_red);
  free(this->band_nir);
  free(this->band_swir1);
  free(this->band_termal);
  free(this->band_swir2);
  free(this->tal);

  free(this->radiance_blue);
  free(this->radiance_green);
  free(this->radiance_red);
  free(this->radiance_nir);
  free(this->radiance_swir1);
  free(this->radiance_termal);
  free(this->radiance_swir2);

  free(this->reflectance_blue);
  free(this->reflectance_green);
  free(this->reflectance_red);
  free(this->reflectance_nir);
  free(this->reflectance_swir1);
  free(this->reflectance_termal);
  free(this->reflectance_swir2);

  free(this->albedo);
  free(this->ndvi);
  free(this->lai);
  free(this->evi);
  free(this->pai);

  free(this->enb_emissivity);
  free(this->eo_emissivity);
  free(this->ea_emissivity);
  free(this->short_wave_radiation);
  free(this->large_wave_radiation_surface);
  free(this->large_wave_radiation_atmosphere);
  free(this->surface_temperature);
  free(this->net_radiation);
  free(this->soil_heat);
};

string Products::radiance_function(MTL mtl)
{
  // https://www.usgs.gov/landsat-missions/using-usgs-landsat-level-1-data-product
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    this->radiance_blue[i] = this->band_blue[i] * mtl.rad_mult[PARAM_BAND_BLUE_INDEX] + mtl.rad_add[PARAM_BAND_BLUE_INDEX];
    this->radiance_green[i] = this->band_green[i] * mtl.rad_mult[PARAM_BAND_GREEN_INDEX] + mtl.rad_add[PARAM_BAND_GREEN_INDEX];
    this->radiance_red[i] = this->band_red[i] * mtl.rad_mult[PARAM_BAND_RED_INDEX] + mtl.rad_add[PARAM_BAND_RED_INDEX];
    this->radiance_nir[i] = this->band_nir[i] * mtl.rad_mult[PARAM_BAND_NIR_INDEX] + mtl.rad_add[PARAM_BAND_NIR_INDEX];
    this->radiance_swir1[i] = this->band_swir1[i] * mtl.rad_mult[PARAM_BAND_SWIR1_INDEX] + mtl.rad_add[PARAM_BAND_SWIR1_INDEX];
    this->radiance_termal[i] = this->band_termal[i] * mtl.rad_mult[PARAM_BAND_TERMAL_INDEX] + mtl.rad_add[PARAM_BAND_TERMAL_INDEX];
    this->radiance_swir2[i] = this->band_swir2[i] * mtl.rad_mult[PARAM_BAND_SWIR2_INDEX] + mtl.rad_add[PARAM_BAND_SWIR2_INDEX];

    if (radiance_blue[i] <= 0)
      this->radiance_blue[i] = NAN;
    if (radiance_green[i] <= 0)
      this->radiance_green[i] = NAN;
    if (radiance_red[i] <= 0)
      this->radiance_red[i] = NAN;
    if (radiance_nir[i] <= 0)
      this->radiance_nir[i] = NAN;
    if (radiance_swir1[i] <= 0)
      this->radiance_swir1[i] = NAN;
    if (radiance_termal[i] <= 0)
      this->radiance_termal[i] = NAN;
    if (radiance_swir2[i] <= 0)
      this->radiance_swir2[i] = NAN;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,RADIANCE," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
}

string Products::reflectance_function(MTL mtl)
{
  // https://www.usgs.gov/landsat-missions/using-usgs-landsat-level-1-data-product
  const float sin_sun = sin(mtl.sun_elevation * PI / 180);

  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    this->reflectance_blue[i] = (this->band_blue[i] * mtl.ref_mult[PARAM_BAND_BLUE_INDEX] + mtl.ref_add[PARAM_BAND_BLUE_INDEX]) / sin_sun;
    this->reflectance_green[i] = (this->band_green[i] * mtl.ref_mult[PARAM_BAND_GREEN_INDEX] + mtl.ref_add[PARAM_BAND_GREEN_INDEX]) / sin_sun;
    this->reflectance_red[i] = (this->band_red[i] * mtl.ref_mult[PARAM_BAND_RED_INDEX] + mtl.ref_add[PARAM_BAND_RED_INDEX]) / sin_sun;
    this->reflectance_nir[i] = (this->band_nir[i] * mtl.ref_mult[PARAM_BAND_NIR_INDEX] + mtl.ref_add[PARAM_BAND_NIR_INDEX]) / sin_sun;
    this->reflectance_swir1[i] = (this->band_swir1[i] * mtl.ref_mult[PARAM_BAND_SWIR1_INDEX] + mtl.ref_add[PARAM_BAND_SWIR1_INDEX]) / sin_sun;
    this->reflectance_termal[i] = (this->band_termal[i] * mtl.ref_mult[PARAM_BAND_TERMAL_INDEX] + mtl.ref_add[PARAM_BAND_TERMAL_INDEX]) / sin_sun;
    this->reflectance_swir2[i] = (this->band_swir2[i] * mtl.ref_mult[PARAM_BAND_SWIR2_INDEX] + mtl.ref_add[PARAM_BAND_SWIR2_INDEX]) / sin_sun;

    if (reflectance_blue[i] <= 0)
      this->reflectance_blue[i] = NAN;
    if (reflectance_green[i] <= 0)
      this->reflectance_green[i] = NAN;
    if (reflectance_red[i] <= 0)
      this->reflectance_red[i] = NAN;
    if (reflectance_nir[i] <= 0)
      this->reflectance_nir[i] = NAN;
    if (reflectance_swir1[i] <= 0)
      this->reflectance_swir1[i] = NAN;
    if (reflectance_termal[i] <= 0)
      this->reflectance_termal[i] = NAN;
    if (reflectance_swir2[i] <= 0)
      this->reflectance_swir2[i] = NAN;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,REFLECTANCE," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
}

string Products::albedo_function(MTL mtl)
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  // https://doi.org/10.1016/j.rse.2017.10.031
  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    float alb = this->reflectance_blue[i] * mtl.ref_w_coeff[PARAM_BAND_BLUE_INDEX] +
                this->reflectance_green[i] * mtl.ref_w_coeff[PARAM_BAND_GREEN_INDEX] +
                this->reflectance_red[i] * mtl.ref_w_coeff[PARAM_BAND_RED_INDEX] +
                this->reflectance_nir[i] * mtl.ref_w_coeff[PARAM_BAND_NIR_INDEX] +
                this->reflectance_swir1[i] * mtl.ref_w_coeff[PARAM_BAND_SWIR1_INDEX] +
                this->reflectance_swir2[i] * mtl.ref_w_coeff[PARAM_BAND_SWIR2_INDEX];

    this->albedo[i] = (alb - 0.03) / (this->tal[i] * this->tal[i]);

    if (albedo[i] <= 0)
      this->albedo[i] = NAN;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,ALBEDO," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
}

string Products::ndvi_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    this->ndvi[i] = (this->reflectance_nir[i] - this->reflectance_red[i]) / (this->reflectance_nir[i] + this->reflectance_red[i]);

    if (ndvi[i] <= -1 || ndvi[i] >= 1)
      ndvi[i] = NAN;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,NDVI," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::pai_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    float pai_value = 10.1 * (this->reflectance_nir[i] - sqrt(this->reflectance_red[i])) + 3.1;

    if (pai_value < 0)
      pai_value = 0;

    this->pai[i] = pai_value;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,PAI," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::lai_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    float savi = ((1 + 0.5) * (this->reflectance_nir[i] - this->reflectance_red[i])) / (0.5 + (this->reflectance_nir[i] + this->reflectance_red[i]));
    this->savi[i] = savi;

    if (!isnan(savi) && savi > 0.687)
      this->lai[i] = 6;
    if (!isnan(savi) && savi <= 0.687)
      this->lai[i] = -log((0.69 - savi) / 0.59) / 0.91;
    if (!isnan(savi) && savi < 0.1)
      this->lai[i] = 0;

    if (lai[i] < 0)
      lai[i] = 0;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,LAI," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::evi_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    float evi_value = 2.5 * ((this->reflectance_nir[i] - this->reflectance_red[i]) / (this->reflectance_nir[i] + (6 * this->reflectance_red[i]) - (7.5 * this->reflectance_blue[i]) + 1));

    if (evi_value < 0)
      evi_value = 0;

    this->evi[i] = evi_value;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,EVI," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::enb_emissivity_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    if (this->lai[i] == 0)
      this->enb_emissivity[i] = NAN;
    else
      this->enb_emissivity[i] = 0.97 + 0.0033 * this->lai[i];

    if ((ndvi[i] < 0) || (lai[i] > 2.99))
      this->enb_emissivity[i] = 0.98;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,ENB_EMISSIVITY," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::eo_emissivity_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {

    if (this->lai[i] == 0)
      this->eo_emissivity[i] = NAN;
    else
      this->eo_emissivity[i] = 0.95 + 0.01 * this->lai[i];

    if ((this->ndvi[i] < 0) || (this->lai[i] > 2.99))
      this->eo_emissivity[i] = 0.98;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,EO_EMISSIVITY," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::ea_emissivity_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
    this->ea_emissivity[i] = 0.85 * pow((-1 * log(this->tal[i])), 0.09);

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,EA_EMISSIVITY," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::surface_temperature_function(MTL mtl)
{
  float k1, k2;
  switch (mtl.number_sensor)
  {
  case 5:
    k1 = 607.76;
    k2 = 1260.56;
    break;

  case 7:
    k1 = 666.09;
    k2 = 1282.71;
    break;

  case 8:
    k1 = 774.8853;
    k2 = 1321.0789;
    break;

  default:
    cerr << "Sensor problem!";
    exit(6);
  }

  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  float surface_temperature_value;
  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    surface_temperature_value = k2 / (log((this->enb_emissivity[i] * k1 / this->radiance_termal[i]) + 1));

    if (surface_temperature_value < 0)
      surface_temperature_value = 0;

    this->surface_temperature[i] = surface_temperature_value;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,SURFACE_TEMPERATURE," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::short_wave_radiation_function(MTL mtl)
{
  float costheta = sin(mtl.sun_elevation * PI / 180);

  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
    this->short_wave_radiation[i] = (1367 * costheta * this->tal[i]) / (mtl.distance_earth_sun * mtl.distance_earth_sun);

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,SHORT_WAVE_RADIATION," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::large_wave_radiation_surface_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    float temperature_pixel = this->surface_temperature[i];
    float surface_temperature_pow_4 = temperature_pixel * temperature_pixel * temperature_pixel * temperature_pixel;
    this->large_wave_radiation_surface[i] = this->eo_emissivity[i] * 5.67 * 1e-8 * surface_temperature_pow_4;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,LARGE_WAVE_RADIATION_SURFACE," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::large_wave_radiation_atmosphere_function(float temperature)
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  float temperature_kelvin = temperature + 273.15;
  float temperature_kelvin_pow_4 = temperature_kelvin * temperature_kelvin * temperature_kelvin * temperature_kelvin;

  for (int i = 0; i < this->height_band * this->width_band; i++)
    this->large_wave_radiation_atmosphere[i] = this->ea_emissivity[i] * 5.67 * 1e-8 * temperature_kelvin_pow_4;

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,LARGE_WAVE_RADIATION_ATMOSPHERE," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::net_radiation_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    this->net_radiation[i] = this->short_wave_radiation[i] -
                             (this->short_wave_radiation[i] * this->albedo[i]) +
                             this->large_wave_radiation_atmosphere[i] - this->large_wave_radiation_surface[i] -
                             (1 - this->eo_emissivity[i]) * this->large_wave_radiation_atmosphere[i];

    if (this->net_radiation[i] < 0)
      this->net_radiation[i] = 0;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,NET_RADIATION," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};

string Products::soil_heat_flux_function()
{
  system_clock::time_point begin, end;
  int64_t general_time, initial_time, final_time;

  begin = system_clock::now();
  initial_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();

  for (int i = 0; i < this->height_band * this->width_band; i++)
  {
    if ((this->ndvi[i] < 0) || this->ndvi[i] > 0)
    {
      float ndvi_pixel_pow_4 = this->ndvi[i] * this->ndvi[i] * this->ndvi[i] * this->ndvi[i];
      this->soil_heat[i] = (this->surface_temperature[i] - 273.15) * (0.0038 + 0.0074 * this->albedo[i]) *
                           (1 - 0.98 * ndvi_pixel_pow_4) * this->net_radiation[i];
    }
    else
      this->soil_heat[i] = 0.5 * this->net_radiation[i];

    if (this->soil_heat[i] < 0)
      this->soil_heat[i] = 0;
  }

  end = system_clock::now();
  general_time = duration_cast<nanoseconds>(end - begin).count();
  final_time = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
  return "SERIAL,SOIL_HEAT_FLUX," + std::to_string(general_time) + "," + std::to_string(initial_time) + "," + std::to_string(final_time) + "\n";
};
