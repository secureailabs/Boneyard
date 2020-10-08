// Event handlers for dynamic window resizing
$(window).on('load', function() {

  // Banner image resizing
  // Annotate banner images with their native dimensions
  annotateImageDimensions();

  // Register window resize event handlers
  window.addEventListener('resize', resizeTopBannerImages);
  window.addEventListener('resize', resizeSectionBannerImages);

  resizeTopBannerImages();
  resizeSectionBannerImages();

});

// Annotates banner images with their original (native) dimensions
function annotateImageDimensions() {
  $('.top-banner-image, .section-banner-image').each(function(index, object) {
    $(object).data('nativeWidth', $(object).prop('width'));
    $(object).data('nativeHeight', $(object).prop('height'));
  });
};

var TOP_BANNER_HEIGHT_WIDE = 719; // px
var MIN_TOP_BANNER_UPDATED_HEIGHT = 753; // px
var TOP_BANNER_HEIGHT_MEDIUM = 864; // px
// TODO mobile display
var TOP_BANNER_HEIGHT_NARROW = 864; // px
function resizeTopBannerImages() {
  var updatedWidth;
  var updatedHeight;
  var windowWidth = $(window).width();

  $('.top-banner-image').each(function(index, object) {
    updatedWidth = windowWidth;
    updatedHeight = $(object).data('nativeHeight') * (updatedWidth / $(object).data('nativeWidth'));

    if (updatedHeight < MIN_TOP_BANNER_UPDATED_HEIGHT) {
      updatedHeight = TOP_BANNER_HEIGHT_MEDIUM;
      updatedWidth = $(object).data('nativeWidth') * (updatedHeight / $(object).data('nativeHeight'));
      $(object).prop('width', updatedWidth);
      $(object).prop('height', updatedHeight);
      $(object).css('top', 0);
      $(object).css('left', -(updatedWidth - windowWidth) / 2);
    } else {
      $(object).prop('width', updatedWidth);
      $(object).prop('height', updatedHeight);
      $(object).css('top', -(updatedHeight - TOP_BANNER_HEIGHT_WIDE) / 2);
      $(object).css('left', 0);
    }
  });
};

var SECTION_BANNER_HEIGHT_WIDE = 514; // px
var MIN_SECTION_BANNER_UPDATED_HEIGHT = 531; // px
var SECTION_BANNER_HEIGHT_MEDIUM = 608; // px
function resizeSectionBannerImages() {
  var updatedWidth;
  var updatedHeight;
  var windowWidth = $(window).width();

  $('.section-banner-image').each(function(index, object) {
    updatedWidth = windowWidth;
    updatedHeight = $(object).data('nativeHeight') * (updatedWidth / $(object).data('nativeWidth'));

    if (updatedHeight < MIN_SECTION_BANNER_UPDATED_HEIGHT) {
      updatedHeight = SECTION_BANNER_HEIGHT_MEDIUM;
      updatedWidth = $(object).data('nativeWidth') * (updatedHeight / $(object).data('nativeHeight'));
      $(object).prop('width', updatedWidth);
      $(object).prop('height', updatedHeight);
      $(object).css('top', 0);
      $(object).css('left', -(updatedWidth - windowWidth) / 2);
    } else {
      $(object).prop('width', updatedWidth);
      $(object).prop('height', updatedHeight);
      $(object).css('top', -(updatedHeight - SECTION_BANNER_HEIGHT_WIDE) / 2);
      $(object).css('left', 0);
    }
  });
};