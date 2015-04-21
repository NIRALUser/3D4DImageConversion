CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
CMAKE_POLICY(VERSION 2.6)
PROJECT(3DTo4DImages)


#####################     ITK    ############################################
set(ITK_INCLUSION_MODULES 
ITKCommon
ITKIOImageBase
    # IO Components
    ITKIOBMP
    ITKIOBioRad
    ITKIOGDCM
    ITKIOGIPL
    ITKIOJPEG
    ITKIOLSM
    ITKIOMeta
    ITKIONIFTI
    ITKIONRRD
    ITKIOPNG
    ITKIOStimulate
    ITKIOTIFF
    ITKIOGE
)

find_package(GenerateCLP REQUIRED)
if(GenerateCLP_FOUND)
  include(${GenerateCLP_USE_FILE})
else(GenerateCLP_FOUND)
   message(FATAL_ERROR "GenerateCLP Not Found")
endif(GenerateCLP_FOUND)

find_package(ITK 4.7 COMPONENTS ${ITK_INCLUSION_MODULES} REQUIRED )
include(${ITK_USE_FILE})

set(Filter 3DTo4DImages)

set(${Filter}_SOURCE ${Filter}.cxx)
GENERATECLP(${Filter}_SOURCE ${Filter}.xml)
add_executable( ${Filter} ${${Filter}_SOURCE})
target_link_libraries( ${Filter}  ${ITK_LIBRARIES})

