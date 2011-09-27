set(DOCUMENTATION "This module contains classes that compute statistics on
images, or use those statistics to perform image operations. In particular you
will find here: how to calculate image moments, minimum maximum values,
projections, principal components analysis (PCA) for shape modeling,
computation of label statistics and image accumulation.")

itk_module(ITKImageStatistics
  DEPENDS
    ITKImageFilterBase
    ITKStatistics
    ITKSpatialObjects
    ITKImageCompose
  TEST_DEPENDS
    ITKImageIntensity
    ITKTestKernel
    ITKThresholding
    ITKImageLabel
  DESCRIPTION
    "${DOCUMENTATION}"
)

# Extra test dependency on Thresholding is introduced by itkNormalizedCorrelationImageFilterTest;
# Extra test dependency on ImageLabel is introduced by itkBinaryProjectionImageFilterTest.
