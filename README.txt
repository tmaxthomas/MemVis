Contributors: Glenn Smith (smithg11), Max Thomas (thomat5)

Dependencies:
    Linux/macOS
    DynamoRIO (see note for additional info)
    Qt 5.10.1 or later

Directories:
    drcachetrace:
        Contains the modified version of DrCacheTrace used for data collection
    tests:
        Contains several test programs used to verify functionality of the tool
    vizFile:
        Contains file-parsing code
    vizRenderQt:
        Contains UI code for theh visualization

Test descriptions:
    randomDataAccess:
        Uses non-deterministic accesses to cause poor cache performance
    hurtMyCacheGood:
        Deterministically produces a large region with as-low-as-possible
        cache performance
    memMessageWriter:
        Hammers certain arranged bits in order to produce patterns in the
        visualization.

Notes: Building the project will require a DynamoRIO install in the same directory
as the folder containing the project. Running the project will require the user to
set DRROOT


