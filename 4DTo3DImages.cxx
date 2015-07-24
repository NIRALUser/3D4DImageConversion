#include "4DTo3DImagesCLP.h"
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
  typedef itk::ImageFileReader< Image4DType > ReaderType ;
  typename ReaderType::Pointer reader = ReaderType::New() ;
  reader->SetFileName( inputVolume ) ;
  reader->Update() ;
  typename Image4DType::PointType iorigin = reader->GetOutput()->GetOrigin() ;
  typename Image4DType::SizeType isize = reader->GetOutput()->GetLargestPossibleRegion().GetSize() ;
  typename Image4DType::DirectionType idirection = reader->GetOutput()->GetDirection() ;
  typename Image4DType::SpacingType ispacing = reader->GetOutput()->GetSpacing() ;
  
  typename VectorImageType::PointType oorigin ;
  typename VectorImageType::SizeType osize ;
  typename VectorImageType::DirectionType odirection ;
  typename VectorImageType::SpacingType ospacing ;
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
  typename VectorImageType::Pointer outputImage = VectorImageType::New() ;
  outputImage->SetOrigin( oorigin ) ;
  outputImage->SetSpacing( ospacing ) ;
  outputImage->SetDirection( odirection ) ;
  outputImage->SetRegions( osize ) ;
  outputImage->SetVectorLength( isize[ 3 ] ) ;
  outputImage->Allocate() ;
  itk::ImageRegionIteratorWithIndex< Image4DType > it( reader->GetOutput() , reader->GetOutput()->GetLargestPossibleRegion().GetSize() ) ;
  itk::VariableLengthVector<PixelType> pixel ;
  for( it.GoToBegin() ; !it.IsAtEnd() ; ++it )
  {
    typename Image4DType::IndexType inputIndex ;
    inputIndex = it.GetIndex() ;
    typename VectorImageType::IndexType outputIndex ;
    for( int i = 0 ; i < 3 ; i++ )
    {
      outputIndex[ i ] = inputIndex[ i ] ;
    }
    PixelType value = it.Get() ;
    pixel = outputImage->GetPixel( outputIndex ) ;
    pixel.SetElement( inputIndex[ 3 ] , value ) ;
    outputImage->SetPixel( outputIndex , pixel ) ;
  }

  typedef itk::ImageFileWriter< VectorImageType > WriterType ;
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


