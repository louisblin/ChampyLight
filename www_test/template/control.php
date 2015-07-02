<?php 
/** 
 *  User: louisblin
 *  Date: 28/06/2015
 *  Time: 15:10
 *
 *  Pre condition: the array `channels` contains the values of all the channels
 *                 that have to be displayed.
 *
 *  Description:   control.php renders the Control page of the ChampyLight controller.
 */

// Include table of predefined constants
require_once 'constants.php';

$title = 'Control';

ob_start();

?>
<div class="container">

    <!-- HEADER -->
    <div class="row" id="header">
        <h1 class="col-xs-12" id="header_title">ChampyLight Controller</h1>
    </div>

    <form action="submit_values.php" method="POST">
        <!-- Command block to fire transiitons and set timer -->
        <div class="row" id="command">
                
            <!-- Fire transition -->
            <div class="col-sm-4 col-xs-12" id="fire_transition">
                <button type="submit" class="btn btn-default btn-lg">
                      <span class="glyphicon glyphicon-play" aria-hidden="true"> Fire transition</span>
                  </button>
            </div>
            
            <!-- Timer -->
            <div class="col-sm-8 col-xs-12" id="timer">
                <input id="slider_timer" 
                       type="text" 
                       data-slider-min="1" 
                       data-slider-max="1800" 
                       data-slider-step="1" />
                       <span id="slider_timer_msg"> 
                           <i class="fa fa-clock-o">  <span id="slider_timer_value">5</span>''</i>
                       </span>
            </div>

        </div>
        <!-- List of channels that are in the patch -->
        <div class="row">

        <?php foreach ($channels as $key=>$channel): 
            

        // Computing values for coming channel 
        $current_channels = array_slice($channels, $key, CHUNCK_SIZE);
        $channel_no = $channel['channel'];

        // Standards for BW
        $data_slider_enabled = "false";
        $data_slider_red_value = 255;
        $data_slider_green_value = 255;
        $data_slider_blue_value = 255;
        $data_slider_white_value = 255;
        $data_slider_intensity_value = $channel['intensity'];

        // If is RGB, update
        if (strcmp($channel['type'], TYPE_RGB) == 0) {
            $data_slider_enabled = "true";

            $data_slider_red_value = $current_channels[RED_INDEX]['intensity'];
            $data_slider_green_value = $current_channels[GREEN_INDEX]['intensity']; 
            $data_slider_blue_value = $current_channels[BLUE_INDEX]['intensity']; 
            $data_slider_white_value = $current_channels[WHITE_INDEX]['intensity']; 
            $data_slider_intensity_value = $channel[INTENSITY_INDEX]['intensity'];

        }

        ?>
            
            <div class="channel channel_<?php echo $channel_no ?> col-lg-4 col-sm-6 col-xs-12">
                <div class="channel_content row">
                    <div class="channel_side channel_side_left col-xs-2">
                        <div class="channel_no col-xs-12">
                            <i class="fa fa-sliders">  <?php echo $channel_no ?></i>
                        </div>

                        <div class="colour col-xs-offset-2 col-xs-8" id="chan_col_<?php echo $channel_no ?>">
                            
                        </div>  
                        
                        <div class="val_int col-xs-12">
                            <p><i class="fa fa-bolt"> <span id="val_int_<?php echo $channel_no ?>"><?php echo $data_slider_intensity_value ?></span></i></p>
                        </div>
                        
                    </div>
                    
                    <div class="sliders col-xs-8">
                        <p class="slider_red">
                        <input type="text" 
                               class="span2 channel_<?php echo $channel_no ?>" 
                               value="" 
                               data-slider-min="0" 
                               data-slider-max="255" 
                               data-slider-step="1" 
                               data-slider-value="<?php echo $data_slider_red_value ?>" 
                               data-slider-id="RC_<?php echo $channel_no ?>" 
                               id="R_<?php echo $channel_no ?>" 
                               data-slider-enabled="<?php echo $data_enabled?>"
                               data-slider-handle="round" />
                        </p>
                        <p class="slider_green">
                        <input type="text" 
                               class="span2 channel_<?php echo $channel_no ?>" 
                               value="" 
                               data-slider-min="0" 
                               data-slider-max="255" 
                               data-slider-step="1" 
                               data-slider-value="<?php echo $data_slider_green_value ?>" 
                               data-slider-id="GC_<?php echo $channel_no ?>" 
                               id="G_<?php echo $channel_no ?>" 
                               data-slider-enabled="<?php echo $data_enabled?>"
                               data-slider-handle="round" />
                        </p>
                        <p class="slider_blue">
                        <input type="text" 
                               class="span2 channel_<?php echo $channel_no ?>" 
                               value="" 
                               data-slider-min="0" 
                               data-slider-max="255" 
                               data-slider-step="1" 
                               data-slider-value="<?php echo $data_slider_green_value ?>" 
                               data-slider-id="BC_<?php echo $channel_no ?>" 
                               id="B_<?php echo $channel_no ?>" 
                               data-slider-enabled="<?php echo $data_enabled?>"
                               data-slider-handle="round" />
                        </p>
                        <p class="slider_white">
                        <input type="text" 
                               class="span2 channel_<?php echo $channel_no ?>" 
                               value="" 
                               data-slider-min="0" 
                               data-slider-max="255" 
                               data-slider-step="1" 
                               data-slider-value="<?php echo $data_slider_white_value ?>" 
                               data-slider-id="WC_<?php echo $channel_no ?>" 
                               id="W_<?php echo $channel_no ?>" 
                               data-slider-enabled="<?php echo $data_enabled?>"
                               data-slider-handle="round" />
                        </p>
                    </div>
                    
                    <div class="channel_side channel_side_right col-xs-2">
                        <div class="intensity col-xs-12">
                            <input id="int_<?php echo $channel_no ?>" 
                                   class="channel_<?php echo $channel_no ?>" 
                                   type="text" 
                                   data-slider-min="0" 
                                   data-slider-max="255" 
                                   data-slider-step="1" 
                                   data-slider-value="<?php $data_slider_intensity_value ?>" 
                                   data-slider-orientation="vertical"/>
                        </div>
                    </div>
                </div>
            </div>

            <?php endforeach ?>
            <!-- Foreach loop end -->    
        </div>
    </form>

    <!-- FOOTER -->
    <div class="row" id="footer">
        <div class="col-xs-12 btn-group btn-group-lg" role="group">
            <button type="button" id="control_btn" class="disabled col-xs-6 btn btn-default">Control</button>
            <button type="button" id="patch_btn" class="col-xs-6 btn btn-default">Patch</button>
        </div>
    </div>

</div>

<!-- Scripts for sliders-->
<script type='text/javascript' src="template/bootstrap-slider/js/bootstrap-slider.js"></script>
<script type='text/javascript' src="template/js/control.js"></script>

<?php $content = ob_get_clean(); ?>

<?php require 'layout.php'; ?>
