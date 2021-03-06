/*****************************************************************************/
/*******************************  CONTROL PAGE *******************************/
/*****************************************************************************/
/**
 *  Author: Louis Blin
 *  Date:   28/06/2015
 *
 *  Description: Javascript utilities for the `control` page of the ChampyLight
 *               project.
 */

/************  ON PAGE LOAD ***********/

// INITIALISING SLIDERS

function init_sliders() {

    // RGB sliders
    $('input')
        .filter(function() {
            return this.id.match(/[WRBG]_.*/);
        })
        .slider({
            formatter: function(value) {
                return Math.floor(value / 2.55) + '%';
            }
        });


    // Intensity sliders
    $('input')
        .filter(function() {
            return this.id.match(/int_.*/);
        })
        .slider({
            formatter: function(value) {
                return Math.floor(value / 2.55) + '%';
            },
            
            reversed : true,
            tooltip : "hide"
        });
}

// SETTING BACKGROUNDS

// Utils
function updateRGBForChannel (channel) {

    // Get values of the 3 associated sliders
    var r = document.getElementById("R_" + channel).value;
    var g = document.getElementById("G_" + channel).value;
    var b = document.getElementById("B_" + channel).value;

    // Update css of render zone 
    $("#chan_col_" + channel)
        .css('background', 'rgb(' + r + ',' + g + ',' + b + ')');

}

function set_color_backgrounds() {
	
    $('div')
        .filter(function() {
            return this.id.match(/chan_col_.*/);
        })
        .each(function() {
            var current_channel = this.id.replace("chan_col_", "");
            updateRGBForChannel(current_channel);            
        });    
}

// SETTING INTENSITY VALUES

function set_intensity_values() {

    $('span')
        .filter(function() {
            return this.id.match(/val_int_.*/);
        })
        .each(function() {
            
            var current_channel = this.id.replace("val_int_", "");

            var value = document.getElementById("int_" + current_channel).value;

            $("#val_int_" + current_channel)
                .text(Math.floor(value / 2.55))
        });   

    // GM
    $("#slider_gm_value")
        .text(Math.floor($('#slider_gm').attr('value') / 2.55));
}

window.onload = function() {
    
    init_sliders();
    set_color_backgrounds();
    set_intensity_values();

    // Close success flash message
    setTimeout(function () {
    
        $('#flash_messages .alert-success').alert('close');
    
    }, 2000);
    
    // To ask the user if he really wants to change page after having modified
    // the sliders values
    has_user_updated_sliders = false;
};

/************* TRANS TYPE *************/

function changeToType(newType) {

    $("#transition_type_view")
        .text(newType.toLowerCase().replace(/\b[a-z]/g, function(letter) {
            return letter.toUpperCase();
        })
    );
    $("#transition_type").attr('value', newType);
}

/***************  TIMER ***************/

$("#slider_timer").slider({
    min: 0,
    max: 255,
    scale: 'logarithmic',
    step: 1
});

$("#slider_timer").on("slide", function(slideEvt) {
    if (slideEvt.value >= 60) {
        var minutes = Math.floor(slideEvt.value / 60);
        var seconds = slideEvt.value % 60;

        $("#slider_timer_value").text(minutes + "'" + seconds);
    }
    else {
        $("#slider_timer_value").text(slideEvt.value);
    }

    // Update value attribute of input
    $("#slider_timer").attr('value', slideEvt.value);
});

/****************** GM ******************/

$('#slider_gm').slider({
    formatter: function(value) {
        return Math.floor(value / 2.55) + '%';
    }
});

$("#slider_gm").on("slide", function(slideEvt) {

    has_user_updated_sliders = true;
    
    // Update value
    $("#slider_gm_value").text(Math.floor(slideEvt.value / 2.55));
    $("#slider_gm").attr('value', slideEvt.value);
});

/***************  SLIDERS ***************/

function getChannelForEvent(slideEvt) {
    
    $.each(slideEvt.currentTarget.classList, function(index, elem) {
        
        if (elem.match(/channel_.*/)) {
            current_channel = elem;
        }
    });
    return current_channel.replace("channel_", "");
}

/** RGB **/

// Pre: Both the sliders and their render zone have the same class name of
//  format `channel_[0-9]+`, and only a single one.

function updateRGB (slideEvt) {

    has_user_updated_sliders = true;
    
    // Get channel involved
    updateRGBForChannel(getChannelForEvent(slideEvt));
}

$('input')
    .filter(function() {
        return this.id.match(/[RBG]_.*/);
    })
    .on("slide", this, updateRGB);
            

/** INTENSITY **/

function updateIntensity(slideEvt) {
    
    has_user_updated_sliders = true;

    // Get channel involved
    var current_channel = getChannelForEvent(slideEvt);

    $("#val_int_" + current_channel).text(Math.floor(slideEvt.value / 2.55));
}

$('input')
    .filter(function() {
        return this.id.match(/int_.*/);
    })
    .on("slide", event, updateIntensity);

/*************** PATCH/CONTROL buttons ***************/

document.getElementById("patch_btn").addEventListener("click", function() {
  
    if (has_user_updated_sliders &&
        ! confirm("Warning: all changes made on this page will be lost. Continue?")) {

        return;
    }

    // Redirecting to Patch
    window.location.href = "patch";

});
