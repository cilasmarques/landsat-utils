#pragma once

#include "constants.h"

/**
 * @brief  Saves a TIFF file.
 * 
 * @param path: Path to save the TIFF file.
 * @param data: Data to be saved.
 * @param height: Height of the data.
 * @param width: Width of the data.
 */
void saveTiff(string path, float *data, int height, int width);
