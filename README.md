# 1.algorithm.cpp
Extract water in Sentinel 2 image using NDWI.
## Input
- NDWI thresholds that distinguish between water and background
- Band3 of Sentinel 2 image(tif)
- Band8 of Sentinel 2 image(tif)
## Output
- NDWI image(tif)

# 2.arcpy.py
Extract flood region using ndwi images before the disaster and after the disaster   
## Input
- inRaster1: ndwi result before the disaster(tif)
- inRaster2: ndwi result after the disaster(tif)
## Output
- eraseOutput: flood region image(tif)
