# Name: reclassify_example02.py
# Description: Reclassifies the values in a raster.
# Requirements: Spatial Analyst Extension

# Import system modules
import arcpy
from arcpy import env
from arcpy.sa import *
import os
import sys
import time

# workspace
workspace = r"D:\Paper\result"
env.workspace = workspace

# Reclassification variables, beforeTime and afterTime is the time before the disaster and after the disaster,
# inRaster1 and inRaster2 is ndwi result before the disaster and after the disaster           
inRaster1 = os.path.join(workspace, "before.tif") 
inRaster2 = os.path.join(workspace, "after.tif") 
reclassField = "Value"
remap = RemapValue([[0, 0]])

# Rasterize vectorized variables
inRaster3 = os.path.join(workspace, "before_beforeTime.tif")
before = os.path.join(workspace, "before_beforeTime.shp")
field = "VALUE"

inRaster4 = os.path.join(workspace, "after_afterTime.tif")
after = os.path.join(workspace, "after_afterTime.shp")

# Erase variable

eraseOutput = os.path.join(workspace, "erase_beforeTime_afterTime.shp")

# area Calculation variable

calculate_output = os.path.join(workspace, "calculate_beforeTime_afterTime.shp")

# filtering variables
in_features = os.path.join(workspace, "cityAndCounty_beforeTime_afterTime.shp") 
out_feature_class = os.path.join(workspace, "majorrdsClass4_beforeTime_afterTime.shp")  
where_clause = '"NAME_1" =\''+sys.argv[1]+'\'' #���޸�

# Spatial link variable

targetFeatures = out_feature_class
joinFeatures = calculate_output
 
# Output will be the target features, states, with a mean city population field (mcp)
outfc = os.path.join(workspace, "join_beforeTime_afterTime.shp")

# Check out the ArcGIS Spatial Analyst extension license
arcpy.CheckOutExtension("Spatial")

# Execute Reclassify
outReclassify1 = Reclassify(inRaster1, reclassField, remap, "NODATA")

# Save the output 
outReclassify1.save(inRaster3)

# Execute Reclassify
outReclassify2 = Reclassify(inRaster2, reclassField, remap, "NODATA")

# Save the output 
outReclassify1.save(inRaster4)

# Execute RasterToPolygon
arcpy.RasterToPolygon_conversion(inRaster3, before, "NO_SIMPLIFY", field)

arcpy.RasterToPolygon_conversion(inRaster4, after, "NO_SIMPLIFY", field)

xyTol = "1 Meters"
arcpy.Erase_analysis(after,before, eraseOutput, xyTol)
 
    # Process: Calculate Areas...
arcpy.CalculateAreas_stats(eraseOutput, calculate_output)

# # Execute Select
# arcpy.Select_analysis(in_features, out_feature_class, where_clause)

# # Create a new fieldmappings and add the two input feature classes.
# fieldmappings = arcpy.FieldMappings()
# fieldmappings.addTable(targetFeatures)
# fieldmappings.addTable(joinFeatures)
 
# # First get the POP1990 fieldmap. POP1990 is a field in the cities feature class.
# # The output will have the states with the attributes of the cities. Setting the
# # field's merge rule to mean will aggregate the values for all of the cities for
# # each state into an average value. The field is also renamed to be more appropriate
# # for the output.
# pop1990FieldIndex = fieldmappings.findFieldMapIndex("F_AREA")
# fieldmap = fieldmappings.getFieldMap(pop1990FieldIndex)
 
# # Get the output field's properties as a field object
# field = fieldmap.outputField
 
# # Rename the field and pass the updated field object back into the field map
# field.name = "sum_area"
# field.aliasName = "total_area"
# fieldmap.outputField = field
 
# # Set the merge rule to mean and then replace the old fieldmap in the mappings object
# # with the updated one
# fieldmap.mergeRule = "Sum"
# fieldmappings.replaceFieldMap(pop1990FieldIndex, fieldmap)
 
# #Run the Spatial Join tool, using the defaults for the join operation and join type
# arcpy.SpatialJoin_analysis(targetFeatures, joinFeatures, outfc, "#", "#", fieldmappings)

print("All done")
