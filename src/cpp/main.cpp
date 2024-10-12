#include <fstream>
#include <iostream>

#include "utils.h"
#include "landsat.h"
#include "constants.h"
#include "parameters.h"

/** 
 * @brief Main function
 * This function is responsible for reading the input parameters and calling the Landsat class to process the products.
 * 
 * @param argc Number of input parameters
 * @param argv Input parameters
 *              - INPUT_BAND_BLUE_INDEX         = 1;
 *              - INPUT_BAND_GREEN_INDEX        = 2;
 *              - INPUT_BAND_RED_INDEX          = 3;
 *              - INPUT_BAND_NIR_INDEX          = 4;
 *              - INPUT_BAND_SWIR1_INDEX        = 5;
 *              - INPUT_BAND_TERMAL_INDEX       = 6;
 *              - INPUT_BAND_SWIR2_INDEX        = 7;
 *              - INPUT_BAND_ELEVATION_INDEX    = 8;
 *              - INPUT_MTL_DATA_INDEX          = 9;
 *              - INPUT_STATION_DATA_INDEX      = 10;
 *              - INPUT_LAND_COVER_INDEX        = 11;
 *              - OUTPUT_FOLDER                 = 12;
 * @return int
*/
int main(int argc, char *argv[])
{
  int INPUT_BAND_ELEV_INDEX    = 8;
  int INPUT_MTL_DATA_INDEX     = 9;
  int INPUT_STATION_DATA_INDEX = 10;
  int OUTPUT_FOLDER            = 11;
  int METHOD_INDEX             = 12;
  int THREADS_INDEX            = 13;

  // Load the meteorologic stations data
  string path_meta_file = argv[INPUT_MTL_DATA_INDEX];
  string station_data_path = argv[INPUT_STATION_DATA_INDEX];

  // Load the landsat images bands
  string bands_paths[INPUT_BAND_ELEV_INDEX];
  for (int i = 0; i < INPUT_BAND_ELEV_INDEX; i++) {
    bands_paths[i] = argv[i+1];
  }

  // Load the SEB model (SEBAL or STEEP) 
  int method = 0;
  if(argc >= METHOD_INDEX){
    string flag = argv[METHOD_INDEX];
    if(flag.substr(0, 6) == "-meth=")
      method = flag[6] - '0';
  }

  int WIDTH = 120;
  int HEIGHT = 120;

  // =====  START + TIME OUTPUT =====
  MTL mtl = MTL(path_meta_file);
  Station station = Station(station_data_path, mtl.image_hour);
  Landsat landsat = Landsat(bands_paths, mtl, 1); // serial implementation

  landsat.compute_Rn_G(station);
  landsat.select_endmembers(method, HEIGHT, WIDTH);

  std::cout << "HOT_COL: " << landsat.hot_pixel.col << std::endl;
  std::cout << "HOT_LINE: " << landsat.hot_pixel.line << std::endl;
  std::cout << "COLD_COL: " << landsat.cold_pixel.col << std::endl;
  std::cout << "COLD_LINE: " << landsat.cold_pixel.line << std::endl;

  float *band_blue;
  float *band_green;
  float *band_red;
  float *band_nir;
  float *band_swir1;
  float *band_termal;
  float *band_swir2;
  float *elevation;

  int initial_line = landsat.cold_pixel.line;   // 43
  int final_line = initial_line + WIDTH;        // 193

  int initial_col = landsat.cold_pixel.col;
  int final_col = initial_col + HEIGHT;

  band_blue = (float *)malloc(HEIGHT * WIDTH * sizeof(float));
  band_green = (float *)malloc(HEIGHT * WIDTH * sizeof(float));
  band_red = (float *)malloc(HEIGHT * WIDTH * sizeof(float));
  band_nir = (float *)malloc(HEIGHT * WIDTH * sizeof(float));
  band_swir1 = (float *)malloc(HEIGHT * WIDTH * sizeof(float));
  band_termal = (float *)malloc(HEIGHT * WIDTH * sizeof(float));
  band_swir2 = (float *)malloc(HEIGHT * WIDTH * sizeof(float));
  elevation = (float *)malloc(HEIGHT * WIDTH * sizeof(float));

  for (int i = initial_line; i < final_line; i++) {
    for (int j = initial_col; j < final_col; j++) {
      band_blue[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.band_blue[i * landsat.width_band + j];
      band_green[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.band_green[i * landsat.width_band + j];
      band_red[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.band_red[i * landsat.width_band + j];
      band_nir[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.band_nir[i * landsat.width_band + j];
      band_swir1[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.band_swir1[i * landsat.width_band + j];
      band_termal[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.band_termal[i * landsat.width_band + j];
      band_swir2[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.band_swir2[i * landsat.width_band + j];
      elevation[(i - initial_line) * WIDTH + (j - initial_col)] = landsat.products.elevation[i * landsat.width_band + j];
    }
  }

  // Save output paths for landsat 8
  string output_folder = argv[OUTPUT_FOLDER];
  saveTiff(output_folder + "/B2.TIF", band_blue, HEIGHT, WIDTH);
  saveTiff(output_folder + "/B3.TIF", band_green, HEIGHT, WIDTH);
  saveTiff(output_folder + "/B4.TIF", band_red, HEIGHT, WIDTH);
  saveTiff(output_folder + "/B5.TIF", band_nir, HEIGHT, WIDTH);
  saveTiff(output_folder + "/B6.TIF", band_swir1, HEIGHT, WIDTH);
  saveTiff(output_folder + "/B10.TIF", band_termal, HEIGHT, WIDTH);
  saveTiff(output_folder + "/B7.TIF", band_swir2, HEIGHT, WIDTH);
  saveTiff(output_folder + "/elevation.tif", elevation, HEIGHT, WIDTH);

  landsat.close();

  return 0;
}