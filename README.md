# Landsat Utilities

A C++ application for processing and analyzing Landsat satellite imagery data. This project provides tools for downloading, preprocessing, and performing surface energy balance calculations on Landsat satellite images.

## Prerequisites

### System Requirements
- Linux operating system
- Docker
- GDAL (Geospatial Data Abstraction Library)
- libtiff development library
- C++14 compatible compiler (g++)

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

