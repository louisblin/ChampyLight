/*****************************************************************************/
/*******************************  PATCH PAGE *********************************/
/*****************************************************************************/
/**
 *  Author: Louis Blin
 *  Date:   28/06/2015
 *
 *  Description: Javascript utilities for the `patch` page of the ChampyLight
 *               project.
 */

/************  ON PAGE LOAD ***********/

function init_adding_modal() {

    // Create modal view
    $('#newPatchModal').modal({
        keyboard: true,
        show: false
    })

}

window.onload = function() {
 
    init_adding_modal();
    $('#field_count').attr('value', 1);
    
    // Close success flash message
    setTimeout(function () {
    
        $('.alert-success').alert('close');
    
    }, 2000);
};

/********* Patch Del / Add  ***********/

// DELETE

function getChannelForEvt(delEvt) {
    
    console.log("Delete event is :");
    console.log(delEvt);
    return delEvt.currentTarget.id.replace("delete_", "");
}


function deletePatch(delEvt) {
    
    var cur_chan = getChannelForEvt(delEvt); 

    if (confirm("Do you really want to delete patch " + cur_chan + "?")) {
        window.location.href = "delete_patch/" + cur_chan;
    }
}

$('button')
    .filter(function() {
        return this.id.match(/delete_.*/);
    })
    .on("click", event, deletePatch);

// ADD

// Show `new patch` modal view on click
$('#newPatchModal').on('shown.bs.modal', function () {
      $('#new_patch_btn').focus()
})

// Add new `dmx input` on click
$('.add_input_btn')
    .on("click", event, function() {
        
        console.log("Add button was pressed");
        
        var row = $(this).closest('tr');
        var clone = row.clone();
    
        // Clear the values
        var tr = clone.closest('tr');
        tr.find('input[type=text]').val('');
    
        $(this).closest('tr').after(clone);
        tr.find("button").hide();

        // Rename new field
        var count = parseInt($('#field_count').attr('value'), 10);
        console.log('Init with count = ' + count);
        $('#field_count').attr('value', count + 1);

        console.log(tr.find('input'));
        
        console.log(tr.find("input[name*='dmx']").attr('name'));
        console.log(' becomes ');
        
        tr.find("input[name*='dmx']").attr('name', 'inputs[' + count + '][dmx]');
        tr.find("input[name*='del']").attr('name', 'inputs[' + count + '][type_del]');
        tr.find("input[name*='led']").attr('name', 'inputs[' + count + '][type_led]');
        
        console.log(tr.find("input[name*='dmx']").attr('name'));
    });

// Toggle selected spotlight type.
$('.spot_type')
    .on("change", function() {
    
        console.log('Updated');
       
        var off = $(this).find("input[value^='off']")[0];
        var on = $(this).find("input[value^='on']")[0];

        $(off).attr("value", "on");
        $(on).attr("value", "off");
    });

/******* PATCH/CONTROL buttons ********/

document.getElementById("control_btn").addEventListener("click", function() {
  
    // Redirecting to Patch
    window.location.href = "control";

});
