## ==== Download and preprocessing
DOCKER_OUTPUT_PATH=/home/saps/output
IMAGES_DIR=./input

## Note: The following variables are used to run the docker containers and are not used in the Makefile itself
#  		 	 The container only retrieves images from between 1984 and 2017. Furthermore, not every day has  
#			 	 images of a specific region because the satellite in orbit collects images every 3 days.
# Samples:
# | landsat_8  | landsat_5  | landsat_7
# | 215065     | 215065     | 215065
# | 2017-05-11 | 1990-03-14 | 2000-12-30
IMAGE_LANDSAT="landsat_8"
IMAGE_PATHROW="215065"
IMAGE_DATE="2017-05-11"

## ==== Execution
METHOD=0
OUTPUT_DATA_PATH=./output
INPUT_DATA_PATH=$(IMAGES_DIR)/$(IMAGE_LANDSAT)_$(IMAGE_PATHROW)_$(IMAGE_DATE)/final_results

clean:
	rm $(OUTPUT_DATA_PATH)/*

clean-all:
	rm -rf $(OUTPUT_DATA_PATH)/*

clean-images:
	rm -rf $(IMAGES_DIR)/*

build-crop:
	g++ -I./include -g ./crop/*.cpp -o ./crop/main -std=c++14 -ltiff

docker-landsat-download:
	docker run \
		-v $(IMAGES_DIR):$(DOCKER_OUTPUT_PATH) \
		-e OUTPUT_PATH=$(DOCKER_OUTPUT_PATH) \
		-e LANDSAT=$(IMAGE_LANDSAT) \
		-e PATHROW=$(IMAGE_PATHROW) \
		-e DATE=$(IMAGE_DATE) \
		cilasmarques/landsat-download:latest

docker-landsat-preprocess:
	docker run \
		-v $(IMAGES_DIR):$(DOCKER_OUTPUT_PATH) \
		-e OUTPUT_PATH=$(DOCKER_OUTPUT_PATH) \
		-e LANDSAT=$(IMAGE_LANDSAT) \
		-e PATHROW=$(IMAGE_PATHROW) \
		-e DATE=$(IMAGE_DATE) \
		cilasmarques/landsat-preprocess:latest

exec-crop-8:
	./crop/main \
		$(INPUT_DATA_PATH)/B2.TIF $(INPUT_DATA_PATH)/B3.TIF $(INPUT_DATA_PATH)/B4.TIF \
		$(INPUT_DATA_PATH)/B5.TIF $(INPUT_DATA_PATH)/B6.TIF $(INPUT_DATA_PATH)/B10.TIF \
		$(INPUT_DATA_PATH)/B7.TIF $(INPUT_DATA_PATH)/elevation.tif $(INPUT_DATA_PATH)/MTL.txt \
		$(INPUT_DATA_PATH)/station.csv $(OUTPUT_DATA_PATH) \
		-meth=$(METHOD) & 

exec-crop-57:
	./crop/main \
		$(INPUT_DATA_PATH)/B1.TIF $(INPUT_DATA_PATH)/B2.TIF $(INPUT_DATA_PATH)/B3.TIF $(INPUT_DATA_PATH)/B4.TIF \
		$(INPUT_DATA_PATH)/B5.TIF $(INPUT_DATA_PATH)/B.TIF \
		$(INPUT_DATA_PATH)/B7.TIF $(INPUT_DATA_PATH)/elevation.tif $(INPUT_DATA_PATH)/MTL.txt \
		$(INPUT_DATA_PATH)/station.csv $(OUTPUT_DATA_PATH) \
		-meth=$(METHOD) & 
