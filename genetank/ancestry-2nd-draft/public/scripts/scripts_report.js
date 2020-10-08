$(document).ready(function() {
  var risksSlideshowIndex = 0;
  var maxRisksSlideshowIndex = $('#risks-slideshow .slide').length - 1;

  console.log(risksSlideshowIndex);
  console.log(maxRisksSlideshowIndex);

  if (risksSlideshowIndex < maxRisksSlideshowIndex) {
    $('#risks-slideshow .arrow-right').removeClass('disabled');
  }

  $('#risks-slideshow .arrow-right').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#risks-' + risksSlideshowIndex).fadeOut(400, function() {
      $('#risks-' + (risksSlideshowIndex + 1)).fadeIn();
      risksSlideshowIndex += 1;
      if (risksSlideshowIndex === 1) {
        $('#risks-slideshow .arrow-left').removeClass('disabled');
      }
      if (risksSlideshowIndex === maxRisksSlideshowIndex) {
        $('#risks-slideshow .arrow-right').addClass('disabled');
      }
    });
  });

  $('#risks-slideshow .arrow-left').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#risks-' + risksSlideshowIndex).fadeOut(400, function() {
      $('#risks-' + (risksSlideshowIndex - 1)).fadeIn();
      risksSlideshowIndex -= 1;
      if (risksSlideshowIndex === 0) {
        $('#risks-slideshow .arrow-left').addClass('disabled');
      }
      if (risksSlideshowIndex === maxRisksSlideshowIndex - 1) {
        $('#risks-slideshow .arrow-right').removeClass('disabled');
      }
    });
  });

  var diseasesSlideshowIndex = 0;
  var maxDiseasesSlideshowIndex = $('#diseases-slideshow .slide').length - 1;

  console.log(diseasesSlideshowIndex);
  console.log(maxDiseasesSlideshowIndex);

  if (diseasesSlideshowIndex < maxDiseasesSlideshowIndex) {
    $('#diseases-slideshow .arrow-right').removeClass('disabled');
  }

  $('#diseases-slideshow .arrow-right').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#diseases-' + diseasesSlideshowIndex).fadeOut(400, function() {
      $('#diseases-' + (diseasesSlideshowIndex + 1)).fadeIn();
      diseasesSlideshowIndex += 1;
      if (diseasesSlideshowIndex === 1) {
        $('#diseases-slideshow .arrow-left').removeClass('disabled');
      }
      if (diseasesSlideshowIndex === maxDiseasesSlideshowIndex) {
        $('#diseases-slideshow .arrow-right').addClass('disabled');
      }
    });
  });

  $('#diseases-slideshow .arrow-left').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#diseases-' + diseasesSlideshowIndex).fadeOut(400, function() {
      $('#diseases-' + (diseasesSlideshowIndex - 1)).fadeIn();
      diseasesSlideshowIndex -= 1;
      if (diseasesSlideshowIndex === 0) {
        $('#diseases-slideshow .arrow-left').addClass('disabled');
      }
      if (diseasesSlideshowIndex === maxDiseasesSlideshowIndex - 1) {
        $('#diseases-slideshow .arrow-right').removeClass('disabled');
      }
    });
  });

  var medicationSlideshowIndex = 0;
  var maxMedicationSlideshowIndex = $('#medication-slideshow .slide').length - 1;

  console.log(medicationSlideshowIndex);
  console.log(maxMedicationSlideshowIndex);

  if (medicationSlideshowIndex < maxMedicationSlideshowIndex) {
    $('#medication-slideshow .arrow-right').removeClass('disabled');
  }

  $('#medication-slideshow .arrow-right').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#medication-' + medicationSlideshowIndex).fadeOut(400, function() {
      $('#medication-' + (medicationSlideshowIndex + 1)).fadeIn();
      medicationSlideshowIndex += 1;
      if (medicationSlideshowIndex === 1) {
        $('#medication-slideshow .arrow-left').removeClass('disabled');
      }
      if (medicationSlideshowIndex === maxMedicationSlideshowIndex) {
        $('#medication-slideshow .arrow-right').addClass('disabled');
      }
    });
  });

  $('#medication-slideshow .arrow-left').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#medication-' + medicationSlideshowIndex).fadeOut(400, function() {
      $('#medication-' + (medicationSlideshowIndex - 1)).fadeIn();
      medicationSlideshowIndex -= 1;
      if (medicationSlideshowIndex === 0) {
        $('#medication-slideshow .arrow-left').addClass('disabled');
      }
      if (medicationSlideshowIndex === maxMedicationSlideshowIndex - 1) {
        $('#medication-slideshow .arrow-right').removeClass('disabled');
      }
    });
  });

  var specialSlideshowIndex = 0;
  var maxSpecialSlideshowIndex = $('#special-slideshow .slide').length - 1;

  console.log(specialSlideshowIndex);
  console.log(maxSpecialSlideshowIndex);

  if (specialSlideshowIndex < maxSpecialSlideshowIndex) {
    $('#special-slideshow .arrow-right').removeClass('disabled');
  }

  $('#special-slideshow .arrow-right').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#special-' + specialSlideshowIndex).fadeOut(400, function() {
      $('#special-' + (specialSlideshowIndex + 1)).fadeIn();
      specialSlideshowIndex += 1;
      if (specialSlideshowIndex === 1) {
        $('#special-slideshow .arrow-left').removeClass('disabled');
      }
      if (specialSlideshowIndex === maxSpecialSlideshowIndex) {
        $('#special-slideshow .arrow-right').addClass('disabled');
      }
    });
  });

  $('#special-slideshow .arrow-left').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#special-' + specialSlideshowIndex).fadeOut(400, function() {
      $('#special-' + (specialSlideshowIndex - 1)).fadeIn();
      specialSlideshowIndex -= 1;
      if (specialSlideshowIndex === 0) {
        $('#special-slideshow .arrow-left').addClass('disabled');
      }
      if (specialSlideshowIndex === maxSpecialSlideshowIndex - 1) {
        $('#special-slideshow .arrow-right').removeClass('disabled');
      }
    });
  });

  var dietSlideshowIndex = 0;
  var maxDietSlideshowIndex = $('#diet-slideshow .slide').length - 1;

  console.log(dietSlideshowIndex);
  console.log(maxDietSlideshowIndex);

  if (dietSlideshowIndex < maxDietSlideshowIndex) {
    $('#diet-slideshow .arrow-right').removeClass('disabled');
  }

  $('#diet-slideshow .arrow-right').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#diet-' + dietSlideshowIndex).fadeOut(400, function() {
      $('#diet-' + (dietSlideshowIndex + 1)).fadeIn();
      dietSlideshowIndex += 1;
      if (dietSlideshowIndex === 1) {
        $('#diet-slideshow .arrow-left').removeClass('disabled');
      }
      if (dietSlideshowIndex === maxDietSlideshowIndex) {
        $('#diet-slideshow .arrow-right').addClass('disabled');
      }
    });
  });

  $('#diet-slideshow .arrow-left').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#diet-' + dietSlideshowIndex).fadeOut(400, function() {
      $('#diet-' + (dietSlideshowIndex - 1)).fadeIn();
      dietSlideshowIndex -= 1;
      if (dietSlideshowIndex === 0) {
        $('#diet-slideshow .arrow-left').addClass('disabled');
      }
      if (dietSlideshowIndex === maxDietSlideshowIndex - 1) {
        $('#diet-slideshow .arrow-right').removeClass('disabled');
      }
    });
  });

  var nutritionSlideshowIndex = 0;
  var maxNutritionSlideshowIndex = $('#nutrition-slideshow .slide').length - 1;

  console.log(nutritionSlideshowIndex);
  console.log(maxNutritionSlideshowIndex);

  if (nutritionSlideshowIndex < maxNutritionSlideshowIndex) {
    $('#nutrition-slideshow .arrow-right').removeClass('disabled');
  }

  $('#nutrition-slideshow .arrow-right').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#nutrition-' + nutritionSlideshowIndex).fadeOut(400, function() {
      $('#nutrition-' + (nutritionSlideshowIndex + 1)).fadeIn();
      nutritionSlideshowIndex += 1;
      if (nutritionSlideshowIndex === 1) {
        $('#nutrition-slideshow .arrow-left').removeClass('disabled');
      }
      if (nutritionSlideshowIndex === maxNutritionSlideshowIndex) {
        $('#nutrition-slideshow .arrow-right').addClass('disabled');
      }
    });
  });

  $('#nutrition-slideshow .arrow-left').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#nutrition-' + nutritionSlideshowIndex).fadeOut(400, function() {
      $('#nutrition-' + (nutritionSlideshowIndex - 1)).fadeIn();
      nutritionSlideshowIndex -= 1;
      if (nutritionSlideshowIndex === 0) {
        $('#nutrition-slideshow .arrow-left').addClass('disabled');
      }
      if (nutritionSlideshowIndex === maxNutritionSlideshowIndex - 1) {
        $('#nutrition-slideshow .arrow-right').removeClass('disabled');
      }
    });
  });

  var exerciseSlideshowIndex = 0;
  var maxExerciseSlideshowIndex = $('#exercise-slideshow .slide').length - 1;

  console.log(exerciseSlideshowIndex);
  console.log(maxExerciseSlideshowIndex);

  if (exerciseSlideshowIndex < maxExerciseSlideshowIndex) {
    $('#exercise-slideshow .arrow-right').removeClass('disabled');
  }

  $('#exercise-slideshow .arrow-right').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#exercise-' + exerciseSlideshowIndex).fadeOut(400, function() {
      $('#exercise-' + (exerciseSlideshowIndex + 1)).fadeIn();
      exerciseSlideshowIndex += 1;
      if (exerciseSlideshowIndex === 1) {
        $('#exercise-slideshow .arrow-left').removeClass('disabled');
      }
      if (exerciseSlideshowIndex === maxExerciseSlideshowIndex) {
        $('#exercise-slideshow .arrow-right').addClass('disabled');
      }
    });
  });

  $('#exercise-slideshow .arrow-left').click(function() {
    if ($(this).hasClass('disabled')) {
      return;
    }
    $('#exercise-' + exerciseSlideshowIndex).fadeOut(400, function() {
      $('#exercise-' + (exerciseSlideshowIndex - 1)).fadeIn();
      exerciseSlideshowIndex -= 1;
      if (exerciseSlideshowIndex === 0) {
        $('#exercise-slideshow .arrow-left').addClass('disabled');
      }
      if (exerciseSlideshowIndex === maxExerciseSlideshowIndex - 1) {
        $('#exercise-slideshow .arrow-right').removeClass('disabled');
      }
    });
  });
});