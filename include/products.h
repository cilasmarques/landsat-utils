#pragma once

#include "utils.h"
#include "candidate.h"
#include "constants.h"
#include "parameters.h"

/**
 * @brief  Struct to manage the products calculation.
 */
struct Products
{
  int nBytes_band;
  uint32_t width_band;
  uint32_t height_band;

  float H_pf_terra;
  float H_pq_terra;
  float rah_ini_pq_terra;
  float rah_ini_pf_terra;

  float *band_blue;
  float *band_green;
  float *band_red;
  float *band_nir;
  float *band_swir1;
  float *band_termal;
  float *band_swir2;
  float *elevation;
  float *tal;

  float *radiance_blue;
  float *radiance_green;
  float *radiance_red;
  float *radiance_nir;
  float *radiance_swir1;
  float *radiance_termal;
  float *radiance_swir2;

  float *reflectance_blue;
  float *reflectance_green;
  float *reflectance_red;
  float *reflectance_nir;
  float *reflectance_swir1;
  float *reflectance_termal;
  float *reflectance_swir2;

  float *albedo;
  float *ndvi;
  float *soil_heat;
  float *surface_temperature;
  float *net_radiation;

  float *savi;
  float *lai;
  float *evi;
  float *pai;
  float *enb_emissivity;
  float *eo_emissivity;
  float *ea_emissivity;
  float *short_wave_radiation;
  float *large_wave_radiation_surface;
  float *large_wave_radiation_atmosphere;

  /**
   * @brief  Constructor.
   */
  Products();

  /**
   * @brief  Constructor.
   * @param  width_band: Band width.
   * @param  height_band: Band height.
   */
  Products(uint32_t width_band, uint32_t height_band);

  /**
   * @brief  Destructor.
   */
  void close();

  /**
   * @brief  The spectral radiance for each band is computed.
   * @param  mtl: MTL struct.
   * @param  sensor: Sensor struct.
   */
  string radiance_function(MTL mtl);

  /**
   * @brief  The spectral reflectance for each band is computed.
   * @param  mtl: MTL struct.
   * @param  sensor: Sensor struct.
   */
  string reflectance_function(MTL mtl);

  /**
   * @brief  The surface albedo is computed.
   * @param  sensor: Sensor struct.
   */
  string albedo_function(MTL mtl);

  /**
   * @brief  The NDVI is computed.
   */
  string ndvi_function();

  /**
   * @brief  The PAI is computed.
   */
  string pai_function();

  /**
   * @brief  The LAI is computed.
   */
  string lai_function();

  /**
   * @brief  The EVI is computed.
   */
  string evi_function();

  /**
   * @brief  The emissivity is computed.
   */
  string enb_emissivity_function();

  /**
   * @brief  The emissivity is computed.
   */
  string eo_emissivity_function();

  /**
   * @brief  The emissivity is computed.
   */
  string ea_emissivity_function();

  /**
   * @brief  The surface temperature is computed.
   */
  string surface_temperature_function(MTL mtl);

  /**
   * @brief  The short wave radiation is computed.
   * @param  mtl: MTL struct.
   */
  string short_wave_radiation_function(MTL mtl);

  /**
   * @brief  The large wave radiation is computed.
   */
  string large_wave_radiation_surface_function();

  /**
   * @brief  The large wave radiation is computed.
   * @param  temperature: Pixel's temperature.
   */
  string large_wave_radiation_atmosphere_function(float temperature);

  /**
   * @brief  The net radiation is computed.
   */
  string net_radiation_function();

  /**
   * @brief  The soil heat flux is computed.
   */
  string soil_heat_flux_function();
};
