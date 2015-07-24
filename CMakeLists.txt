CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
CMAKE_POLICY(VERSION 2.6)
PROJECT(3DTo4DImages)


#####################     ITK    ############################################
set(ITK_INCLUSION_MODULES 
  ITKCommon
  ITKIOImageBase
  # IO Components
  ITKIONRRD
  ITKIONIFTI
  ITKIOMeta
)

find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

find_package(ITK 4.7 COMPONENTS ${ITK_INCLUSION_MODULES} REQUIRED )
include(${ITK_USE_FILE})
set( ITK_LIBRARIES_WITHIO ${ITK_LIBRARIES} )

set(Filters 3DTo4DImages 4DTo3DImages)

foreach(Filter ${Filters})
  SEMMacroBuildCLI(
      NAME ${Filter}
      EXECUTABLE_ONLY
      TARGET_LIBRARIES ${ITK_LIBRARIES_WITHIO}
  )
endforeach()
