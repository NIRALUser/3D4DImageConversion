#include "3DTo4DImagesCLP.h"
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkVectorImage.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIteratorWithIndex.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

//To check the image voxel type
void GetImageType( std::string fileName ,
                   itk::ImageIOBase::IOPixelType &pixelType ,
                   itk::ImageIOBase::IOComponentType &componentType
                 )
{
   typedef itk::Image< unsigned char , 3 > ImageType ;
   itk::ImageFileReader< ImageType >::Pointer imageReader ;
   imageReader = itk::ImageFileReader< ImageType >::New() ;
   imageReader->SetFileName( fileName.c_str() ) ;
   imageReader->UpdateOutputInformation() ;
   pixelType = imageReader->GetImageIO()->GetPixelType() ;
   componentType = imageReader->GetImageIO()->GetComponentType() ;
}

template< class PixelType > 
int LoadAndSave( std::string inputVolume , std::string outputVolume )
{
  typedef itk::VectorImage< PixelType , 3 > VectorImageType ; 
  typedef itk::Image< PixelType , 4 > Image4DType ;
  typedef itk::ImageFileReader< VectorImageType > ReaderType ;
  typename ReaderType::Pointer reader = ReaderType::New() ;
  reader->SetFileName( inputVolume ) ;
  reader->Update() ;
  typename VectorImageType::PointType iorigin = reader->GetOutput()->GetOrigin() ;
  typename VectorImageType::SizeType isize = reader->GetOutput()->GetLargestPossibleRegion().GetSize() ;
  typename VectorImageType::DirectionType idirection = reader->GetOutput()->GetDirection() ;
  typename VectorImageType::SpacingType ispacing = reader->GetOutput()->GetSpacing() ;
  
  typename Image4DType::PointType oorigin ;
  typename Image4DType::SizeType osize ;
  typename Image4DType::DirectionType odirection ;
  typename Image4DType::SpacingType ospacing ;
  odirection.SetIdentity() ;
  for( int i = 0 ; i < 3 ; i++ )
  {
    oorigin[ i ] = iorigin[ i ] ;
    ospacing[ i ] = ispacing[ i ] ;
    osize[ i ] = isize[ i ] ;
    for( int j = 0 ; j < 3 ; j++ )
    {
      odirection[ i ][ j ] = idirection[ i ][ j ] ;
    }
  }
  oorigin[ 3 ] = 0 ;
  ospacing[ 3 ] = 1 ;
  osize[ 3 ] = reader->GetOutput()->GetVectorLength () ;
  typename Image4DType::Pointer outputImage = Image4DType::New() ;
  outputImage->SetOrigin( oorigin ) ;
  outputImage->SetSpacing( ospacing ) ;
  outputImage->SetDirection( odirection ) ;
  outputImage->SetRegions( osize ) ;
  outputImage->Allocate() ;
  itk::ImageRegionIteratorWithIndex< VectorImageType > it( reader->GetOutput() , reader->GetOutput()->GetLargestPossibleRegion().GetSize() ) ;
  for( it.GoToBegin() ; !it.IsAtEnd() ; ++it )
  {
    itk::VariableLengthVector< PixelType > value ;
    value = it.Get() ;
    typename VectorImageType::IndexType inputIndex ;
    inputIndex = it.GetIndex() ;
    typename Image4DType::IndexType outputIndex ;
    for( int i = 0 ; i < 3 ; i++ )
    {
      outputIndex[ i ] = inputIndex[ i ] ;
    }
    for( unsigned int i = 0 ; i < value.GetSize() ; i++ )
    {
      outputIndex[ 3 ] = i ;
      outputImage->SetPixel( outputIndex , value[ i ] ) ;
    }
  }

  typedef itk::ImageFileWriter< Image4DType > WriterType ;
  typename WriterType::Pointer writer = WriterType::New() ;
  writer->SetFileName( outputVolume ) ;
  writer->SetInput( outputImage ) ;
  writer->SetUseCompression( 1 ) ;
  writer->Update() ;
  return EXIT_SUCCESS ;
}


int main( int argc , char * argv[] )
{
   PARSE_ARGS ;
   itk::ImageIOBase::IOPixelType pixelType ;
   itk::ImageIOBase::IOComponentType componentType ;
   GetImageType( inputVolume , pixelType , componentType ) ;
   //templated over the input image voxel type
   switch( componentType )
   {
      case itk::ImageIOBase::UCHAR:
         return LoadAndSave< unsigned char >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::CHAR:
         return LoadAndSave< char >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::USHORT:
         return LoadAndSave< unsigned short >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::SHORT:
         return LoadAndSave< short >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::UINT:
         return LoadAndSave< unsigned int >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::INT:
         return LoadAndSave< int >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::ULONG:
         return LoadAndSave< unsigned long >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::LONG:
         return LoadAndSave< long >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::FLOAT:
         return LoadAndSave< float >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::DOUBLE:
         return LoadAndSave< double >( inputVolume , outputVolume ) ;
         break ;
      case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
      default:
         std::cerr << "unknown component type" << std::endl ;
         break ;
   }
   return EXIT_FAILURE ;
}


