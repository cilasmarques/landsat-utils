## Steps:

#### Install docker
```
curl -fSsL https://get.docker.com | bash
```

#### Install gdal
```
sudo add-apt-repository ppa:ubuntugis/ppa && sudo apt-get update 
sudo apt-get install gdal-bin
```

#### Install tiffs lib
```
sudo apt-get install libtiff5-dev
```

#### Make commands
* docker-landsat-download: 
    * Downloads the specified Landsat image.
* docker-landsat-preprocess: 
    * Preprocesses the Landsat image to ensure it's in the correct format for script input.
* build-cpp:
    * Build the cpp code
* exec-landsat*: 
    * Executes the script while generating an Nsight Systems (nsys) report.
