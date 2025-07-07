# Landsat Utilities

A C++ application for processing and analyzing Landsat satellite imagery data. This project provides tools for downloading, preprocessing, and performing surface energy balance calculations on Landsat satellite images.

## Project Structure

```
landsat-utils/
├── crop/           # C++ application for Landsat processing
├── eval/           # Python scripts for TIFF comparison and evaluation
├── include/        # Header files
├── input/          # Input data directory
├── output/         # Output data directory
```

## Prerequisites

### System Requirements
- Linux operating system
- Docker
- GDAL (Geospatial Data Abstraction Library)
- libtiff development library
- C++14 compatible compiler (g++)
- Python 3.7+ (for evaluation scripts)

### Installation

1. **Install Docker**
   ```bash
   curl -fSsL https://get.docker.com | bash
   ```

2. **Install GDAL**
   ```bash
   sudo add-apt-repository ppa:ubuntugis/ppa && sudo apt-get update 
   sudo apt-get install gdal-bin
   ```

3. **Install libtiff development library**
   ```bash
   sudo apt-get install libtiff5-dev
   ```

4. **Install Python dependencies (for evaluation)**
   ```bash
   cd eval
   pip3 install -r requirements.txt
   ```

---

# Crop Module - Landsat Processing

## Usage

### 1. Build the Application
```bash
make build-crop
```

### 2. Download Landsat Data
Configure the image parameters in the Makefile:
```makefile
IMAGE_LANDSAT="landsat_8"      # landsat_5, landsat_7, or landsat_8
IMAGE_PATHROW="215065"         # Path/Row identifier
IMAGE_DATE="2017-05-11"        # Date in YYYY-MM-DD format
```

Then download the data:
```bash
make docker-landsat-download
```

### 3. Preprocess the Data
```bash
make docker-landsat-preprocess
```

### 4. Execute Processing

#### For Landsat 8:
```bash
make exec-crop-8
```

#### For Landsat 5/7:
```bash
make exec-crop-57
```

### Configuration Parameters

You can customize the execution parameters in the Makefile:

```makefile
METHOD=0              # SEB method (0: SEBAL, 1: STEEP)
OUTPUT_DATA_PATH=./output
INPUT_DATA_PATH=./input/landsat_8_215065_2017-05-11/final_results
```

## Available Make Commands

| Command | Description |
|---------|-------------|
| `build-crop` | Build the C++ application |
| `docker-landsat-download` | Download specified Landsat image |
| `docker-landsat-preprocess` | Preprocess Landsat image for analysis |
| `exec-crop-8` | Execute processing for Landsat 8 data |
| `exec-crop-57` | Execute processing for Landsat 5/7 data |
| `clean` | Clean output files |
| `clean-all` | Clean all output files and directories |
| `clean-images` | Remove all downloaded images |

### Evaluation Commands

| Command | Description |
|---------|-------------|
| `install-eval-deps` | Install Python dependencies for evaluation |
| `exec-eval` | Execute TIFF comparison and evaluation |
| `exec-eval-custom` | Execute evaluation with custom parameters |
| `clean-eval` | Clean evaluation CSV files |

## Output Products

The application outputs **cropped spectral bands** from the original Landsat imagery. The cropping is performed around the cold pixel location identified during endmember selection.

### Output Files (GeoTIFF format)

**For Landsat 8:**
- `B2.TIF` - Blue band (0.452-0.512 μm)
- `B3.TIF` - Green band (0.533-0.590 μm) 
- `B4.TIF` - Red band (0.636-0.673 μm)
- `B5.TIF` - Near Infrared (NIR) band (0.851-0.879 μm)
- `B6.TIF` - Short Wave Infrared 1 (SWIR1) band (1.566-1.651 μm)
- `B10.TIF` - Thermal Infrared 1 (TIRS1) band (10.60-11.19 μm)
- `B7.TIF` - Short Wave Infrared 2 (SWIR2) band (2.107-2.294 μm)
- `elevation.tif` - Digital Elevation Model (DEM)

**For Landsat 5/7:**
- `B1.TIF` - Blue band
- `B2.TIF` - Green band
- `B3.TIF` - Red band
- `B4.TIF` - Near Infrared (NIR) band
- `B5.TIF` - Short Wave Infrared 1 (SWIR1) band
- `B6.TIF` - Thermal Infrared band
- `B7.TIF` - Short Wave Infrared 2 (SWIR2) band
- `elevation.tif` - Digital Elevation Model (DEM)

### Cropping Details
- **Crop size**: 3647 × 3251 pixels (approximately half of original image)
- **Crop location**: Centered around the cold pixel identified during endmember selection
- **Format**: 32-bit floating point GeoTIFF files
- **Coordinate system**: Preserved from original Landsat data

All outputs are saved in the `output/` directory specified in the Makefile.

---

# Eval Module - TIFF Comparison and Evaluation

## Overview

The evaluation module compares TIFF files and generates similarity tables with different precision levels. It compares kernel files with their corresponding serial baseline files.

## Features

- Compares kernel files with their corresponding serial baseline files
- Generates 6 CSV tables with different precision levels:
  - **FLOOR (ignores decimal places)**: 3 files (0.1, 1.0, 5.0)
  - **ROUND (2 decimal places)**: 3 files (0.1, 1.0, 5.0)
- Counts how many occurrences diverge at each precision level
- Calculates the median of differences between corresponding pixels

## File Comparisons

The script compares the following file pairs:
- `kernels-raw-0-evapotranspiration_24h.tif` ↔ `serial-0-evapotranspiration_24h.tif` (steep)
- `kernels-raw-1-evapotranspiration_24h.tif` ↔ `serial-1-evapotranspiration_24h.tif` (sebal)
- `kernels-streams-sync-0-evapotranspiration_24h.tif` ↔ `serial-0-evapotranspiration_24h.tif` (steep)
- `kernels-streams-sync-1-evapotranspiration_24h.tif` ↔ `serial-1-evapotranspiration_24h.tif` (sebal)

## How to Execute

### 1. Install Dependencies

```bash
cd eval
pip3 install -r requirements.txt
```

### 2. Run Comparison

#### Option A: Automatic Script
```bash
# Install dependencies
make install-eval-deps

# Run evaluation
make exec-eval
```

#### Option B: Manual Execution
```bash
python3 compare_tiffs.py
```

#### Option C: With Custom Parameters
```bash
python3 compare_tiffs.py --tiffs_dir ../input --output_dir ./output
```

### 3. Available Parameters

- `--tiffs_dir`: Directory with TIFF files 
- `--output_dir`: Output directory for CSVs 

## Output Files

The script generates **6 CSV files**:

### FLOOR (ignores decimal places):
- `similaridade_floor_01_decimal.csv` - precision 0.1
- `similaridade_floor_1_unidade.csv` - precision 1.0
- `similaridade_floor_5_unidades.csv` - precision 5.0

### ROUND (2 decimal places):
- `similaridade_round_01_decimal.csv` - precision 0.1
- `similaridade_round_1_unidade.csv` - precision 1.0
- `similaridade_round_5_unidades.csv` - precision 5.0

### CSV Columns:
- `Arquivo_Kernel`: Kernel file name
- `Arquivo_Serial`: Corresponding serial file name
- `Pixels_Diferentes`: Number of pixels that diverge
- `Similaridade_%`: Similarity percentage
- `Total_Pixels`: Total pixels in the image
- `Diferença_Mediana`: Median of absolute differences between pixels

----

## Technical Details

### Supported Landsat Missions
- **Landsat 5**: TM sensor (1984-2013)
- **Landsat 7**: ETM+ sensor (1999-present)
- **Landsat 8**: OLI/TIRS sensors (2013-present)

### Processing Methods
- **SEBAL (Surface Energy Balance Algorithm for Land)**: Method 0
- **STEEP (Surface Temperature and Energy Exchange Process)**: Method 1

### Data Requirements
- **Input bands**: 7 spectral bands + elevation data
- **Metadata**: MTL.txt file with calibration parameters
- **Station data**: Meteorological station information
- **Coverage**: Images available every 3 days per region

## License

This project is developed for research and educational purposes in remote sensing and surface energy balance analysis.

## Contributing

Contributions are welcome! Please ensure your code follows the existing style and includes appropriate documentation.

