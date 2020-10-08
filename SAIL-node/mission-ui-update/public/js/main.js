$(window).on('load', function () {
  /* Add Scrolling Animations */
  $('.nav-scroll').click(function () {
    // Update active section
    $('.navbar li').removeClass('active');
    $(this).closest('li').addClass('active');
    // Scrolling animation to section
    $.scrollTo("#" + (this.id.substring("navbar-".length)), 500);
    return false;
  });

  /* Add beta signup form handlers */
  $('#beta-form').submit(function (e) {
    e.preventDefault();

    $.ajax({
      url: '/beta',
      type: 'POST',
      data: { email: $('#beta-email').val() },
      success: function (data) {
        $('.signup-form-status').hide();
        $('#signup-form-success').show();
        $('#beta-email').val('');
      },
      error: function () {
        $('.signup-form-status').hide();
        $('#signup-form-failure').show();
      }
    })
  });
});

function scrollToAnchorString(anchorString) {
  // Scrolling animation to section
  $.scrollTo('#' + anchorString, 500);
}

var mobileNavOpen = false;
$('.navbar').addClass('show-shadow');
function toggleMobileNav() {
  mobileNavOpen = !mobileNavOpen;
  if (mobileNavOpen) {
    $('.navbar').removeClass('show-shadow');
    $('#mobile-nav-links').addClass('open');
  } else {
    $('.navbar').addClass('show-shadow');
    $('#mobile-nav-links').removeClass('open');
  }
}