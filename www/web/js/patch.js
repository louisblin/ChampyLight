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
    
    });

/******* PATCH/CONTROL buttons ********/

document.getElementById("control_btn").addEventListener("click", function() {
  
    // Redirecting to Patch
    window.location.href = "control";

});
