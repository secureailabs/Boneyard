var questions = [
  {
    'q': 'How old are you?',
    'a': ['Under 18', '18-35', '36-55', 'Over 55']
  },
  {
    'q': 'Do you have a history of smoking?',
    'a': ['Yes', 'No']
  }
];
var progress = 0;
var CHOICES = ['a', 'b', 'c', 'd', 'e'];

// if (typeof(Storage) !== 'undefined') {

// }

$(document).ready(function() {
  // Set up first question
  switchQuestion(progress);

  // Event handlers
  $('#next').on('click', function(e) {
    console.log('clicked');
    
    if (answerSelected()) {
      // Disable answering
      $('input[name=answer]').prop('disabled', true);

      // Move to next question
      progress++;
      if (progress >= questions.length) {
        finishQuestions();
      } else {
        // Fade out question
        $('#survey-container').animate({ 'opacity': 0 }, 300, function () {
          // TODO: Report answer to server

          // Clear answer
          $('input[name=answer]').prop('checked', false);
          switchQuestion(progress);

          // Fade in question
          $('#survey-container').animate({ 'opacity': 1 }, 300, function () {
            // Enable answering
            $('input[name=answer]').prop('disabled', false);
          });
        });
      }
    } else {
      notifyRequireAnswer();
    }

  });
});

function switchQuestion(index) {
  $('#question').text(questions[index]['q']);
  for (var i = 0; i < CHOICES.length; i++) {
    if (i >= questions[index]['a'].length) {
      // We don't need the answer choice, hide it
      $('#choice-' + CHOICES[i]).hide();
    } else {
      // Show and populate the answer choice
      $('#choice-' + CHOICES[i]).show();
      $('#label-' + CHOICES[i]).text(questions[index]['a'][i]);
    }
  }
};

function finishQuestions() {
  // Show completion screen
  $('#survey-container').fadeOut(300, function () {
    $('#completion-container').fadeIn(300);
  });
};

function answerSelected() {
  if ($('input[name=answer]:checked').length > 0) {
    return true;
  }
  return false;
};

function notifyRequireAnswer() {
  alert('Please select an answer.');
};