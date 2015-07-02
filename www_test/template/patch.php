<?php 
/** 
 *  User: louisblin
 *  Date: 28/06/2015
 *  Time: 15:10
 *
 *  Pre condition: $channels is a 3D array:
 *                     
 *                     (array of channels (
 *                          array of 2 data information (
 *                             channel: channel number
 *                             array of inputs 2 data info (
 *                                 dmx: dmx address number
 *                                 type: spotlight type)
 *                              )
 *                          )
 *                     )
 *
 *  Description:   patch.php renders the Patch editing page of the ChampyLight controller.
 */

// Include table of predefined constants
require_once 'constants.php';

$title = 'Patch';

ob_start();

?>
<div class="container">

    <!-- HEADER -->
    <div class="row" id="header">
        <h1 class="col-xs-12" id="header_title">ChampyLight Controller</h1>
    </div>
        
    <!-- List of existing patched channels -->
    <div class="row">

    <?php foreach ($channels as $channel_data): 
        
    // Computing values for coming channel 
    $channel_no = $channel_data['channel'];
    $inputs = $channel_data['inputs'];

    ?>
    
        <div class="channel col-lg-2 col-sm-4 col-xs-6">
            <div class="channel_content row">
                <div class="patch_side_left col-xs-4">
                    <div class="channel_no col-xs-12">
                    <i class="fa fa-sliders">  <?php echo $channel_no; ?></i>
                    </div>

                    <div class="delete col-xs-12">
                        <button type="button" class="btn btn-primary" id="delete_<?php echo $channel_no; ?>" aria-label="Delete patch">
                              <span class="glyphicon glyphicon-trash" aria-hidden="true">.</span>
                        </button>
                    </div>  
                    
                </div>
                
                <div class="patch_side_right col-xs-8">

                <?php foreach ($inputs as $input): 
                
                $dmx = $input['dmx'];

                // Standard for BW
                $type = "fa-lightbulb-o";
                
                
                // If RGB
                if (strcmp($input['type'], TYPE_RGB) == 0) {

                    $type = "fa-spinner";
                }
                ?>
                    <div class="col-xs-12">
                        <i class="fa <?php echo $type; ?>">  <?php echo $dmx; ?></i>
                    </div>
                <?php endforeach ?>
                <!-- Input dipslay end -->    

                </div>
                
            </div>
        </div>

        <?php endforeach ?>
        <!-- Channel display end -->    

        <!-- Adding a new patch :: button -->
        <div class="channel col-lg-2 col-sm-4 col-xs-6">
            <!-- Button trigger modal -->
            <button type="button" class="btn btn-primary btn-lg" id="new_patch_btn" 
                                        data-toggle="modal" data-target="#newPatchModal">
                  New patch
            </button>
        </div>

        <!-- Adding a new patch :: modal view -->
        <!-- Modal -->
        <div class="modal fade" id="newPatchModal" tabindex="-1" role="dialog" aria-labelledby="adding_a_new_patch">
            <div class="modal-dialog" role="document">
                <div class="modal-content">
                    
                    <form action="add_patch.php" method="POST">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                                <span aria-hidden="true">&times;</span>
                            </button>
                            <h4 class="modal-title" id="myModalLabel">New patch</h4>
                        </div>
                        
                        <div class="modal-body">
                                
                            <!-- Selecting the channel -->
                            <div class="row channel_div">
                                <div class="input-group">
                                    <span class="input-group-addon" id="basic-addon1">
                                        <i class="fa fa-sliders"></i>
                                    </span>
                                    <input type="text" name="channel_selected" class="form-control" 
                                                placeholder="Channel number" aria-describedby="basic-addon1">
                                </div>
                            </div>
                        
                            <!-- Selecting the inputs -->
                            <div class="row input">
                                <div class="col-xs-12">
                                    <table class="table" id="dmx_inputs">
                                        <tr>
                                            <td>
                                                <input type="text" class="dmx_input" name="input_1" placeholder="DMX (starting) address">
                                            </td>
                                            <td>
                                                <div class="col-xs-6 spot_type btn-group" data-toggle="buttons">
                                                    <label class="btn btn-primary active">
                                                        <input type="radio" name="light_type" id="bulb" checked>
                                                        <i class="fa fa-lightbulb-o">  PAR</i>
                                                    </label>
                                                    <label class="btn btn-primary">
                                                      <input type="radio" name="light_type" id="led">
                                                      <i class="fa fa-spinner">  LED</i>
                                                    </label>
                                                </div>
                                            </td>
                                            <td>
                                                <button type="button" class="btn btn-default add_input_btn">Add</button>
                                            </td>
                                        </tr>
                                    </table>
                                </div>
                            </div>
                        
                        </div>
                        
                        <div class="modal-footer">
                            <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
                            <button type="submit" class="btn btn-primary">Save changes</button>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>

    <!-- FOOTER -->
    <div class="row" id="footer">
        <div class="col-xs-12 btn-group btn-group-lg" role="group">
            <button type="button" id="control_btn" class="col-xs-6 btn btn-default">Control</button>
            <button type="button" id="patch_btn" class="disabled col-xs-6 btn btn-default">Patch</button>
        </div>
    </div>

</div>

<!-- Scripts for patch -->
<script type='text/javascript' src="js/bootstrap.min.js"></script>
<script type='text/javascript' src="js/patch.js"></script>

<?php $content = ob_get_clean(); ?>

<?php require 'layout.php'; ?>
