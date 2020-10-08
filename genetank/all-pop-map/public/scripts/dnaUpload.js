function uploadDNA() {
  var files = document.getElementById('dna-upload').files;

  if (files.length > 0) {
    var formData = new FormData();
    formData.set('dna', files[0], files[0].name);
    if ($('#dna-upload').attr('data-campaign')) {
      formData.set('campaign', $('#dna-upload').data('campaign'));
    }

    $.ajax({
      url: '/uploadDNA',
      type: 'POST',
      data: formData,
      processData: false,
      contentType: false,
      success: function(data) {
        alert('Thank you, your DNA has been successfully uploaded!');
        window.location = window.location.href;
      }
    });
  }
}