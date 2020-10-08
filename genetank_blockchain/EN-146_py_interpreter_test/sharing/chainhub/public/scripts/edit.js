$(document).ready(function () {
  // Form handlers.
  $('#entry-insert-form').submit(entryInsertFormHandler);
  $('#file-upload-form').submit(fileUploadFormHandler);
});

function entryInsertFormHandler(e) {
  e.preventDefault();

  // Construct JSON request.
  try {
    var data = {
      key: document.getElementById('entry-insert-key').value,
      value: JSON.parse(document.getElementById('value').value)
    };
    if (document.getElementById('keywords').value) {
      data['keywords'] = JSON.parse(document.getElementById('keywords'));
    }
  } catch (e) {
    alert(e);
    return;
  }
  data = JSON.stringify(data);

  // Send entry insert request.
  $.ajax({
    url: '/entry/insert',
    type: 'POST',
    data: data,
    contentType: 'application/json',
    success: function (data) {
      if (data.err) {
        alert('Sorry, something went wrong while inserting your entry. Please try again.');
      } else {
        alert('Success! Returned data: ' + JSON.stringify(data));
        // Clear form and refresh data.
        document.getElementById('entry-insert-form').reset();
        getEntries();
      }
    },
    error: function (xhr, ajaxOptions, thrownError) {
      alert(JSON.stringify(xhr));
    }
  });
}

function fileUploadFormHandler(e) {
  e.preventDefault();

  var files = document.getElementById('file').files;
  if (files.length > 0) {
    // Construct form request with file.
    var formData = new FormData();
    formData.set('file', files[0], files[0].name);

    // Send file upload request.
    $.ajax({
      url: '/file/upload',
      type: 'POST',
      data: formData,
      processData: false,
      contentType: false,
      success: function (data) {
        if (data.err) {
          alert('Sorry, something went wrong while uploading your file. Please try again.');
        } else {
          alert('Success! Returned data: ' + JSON.stringify(data));
          // Clear form and refresh data.
          document.getElementById('file-upload-form').reset();
          getEntries();
        }
      }
    });
  }
}
