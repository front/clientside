<?php

/**
 * @file
 *
 * Views Style plugin extension.
 */
class views_plugin_style_transparency extends views_plugin_style {

  /**
   * Set default map options.
   */
  function option_definition() {
    $options = parent::option_definition();
//    $options['add_display']  = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    //xdebug_break();
    $tviews = views_get_enabled_views();
    
    //Herre's the field handlers
    //$handlers = $this->display->handler->get_handlers('field');
    //$field_name = $handlers[0]
    
    // Create an array of allowed columns from the data we know:
    //$field_names = $this->display->handler->get_field_labels();

    $json_views = array();
    foreach ($tviews AS $tview_id => $tview) {
      foreach ($tview->display AS $tdisplay_id => $tdisplay) {
        if (isset($tdisplay->display_options) && $tdisplay->display_options['style_plugin'] == 'views_json') {
          // We have a json-enabled display.


          if (empty($json_views[$tdisplay_id])) {
            $json_views[$tview_id][$tdisplay_id] = $tdisplay->display_title;
          };
        }
      }
    }


    $form['add_display'] = array(
      '#title' => t('Add displays'),
      '#type' => 'select',
      '#multiple' => true,
      '#options' => $json_views,
      '#default_value' => $this->options['add_display'],
    );
    //if (!empty($form_state['values']['add_display'])){
    foreach ($json_views AS $view_id => $view_displays) {
      foreach ($view_displays AS $did => $d) {




          $form['display_' . $did] = array(
            '#type' => 'fieldset',
            '#title' => t('Settings for the display \'@displayid\' at view \'@view\'', array('@view' => $view_id, '@displayid' => $d)),
          );
          $form['display_' . $did]['title'] = array(
            '#type' => 'textfield',
            '#title' => t('Title'),
          );         
          
          $template_fields = array(
            'field_1' => t('Template field 1'),
            'field_2' => t('Template field 2'),
          );
          $fields = array(
            'field_1' => t('Original field 1'),
            'field_2' => t('Original field 2'),
          );
          foreach ($template_fields AS $fid => $fname) {
            $form['display_' . $did]['map_' . $fid] = array(
              '#type' => 'select',
              '#title' => t('Map template field %field to...', array('%field' => $fname)),
              '#options' => $fields,
            );
          }
        }
      
    }
  }

  /**
   * Transform the View result in a list of maker locations and render as a map.
   *
   * @param type $result
   *   This is superfluous, same as $this->view->result.
   *
   */
  function render($result) {

    if (!empty($this->view->live_preview)) {
      return t('You want usable display?? ORLY?');
    }
    else {

      return $output;
    }
  }

  /**
   * Perform token replacement, convert timestamps to date strings etc. for
   * the entire result set. Store the rendered rows on the object.
   *
   * @param $result
   *   The result array from $this->view->result
   */
  function render_fields($result) {
    if (!$this->uses_fields()) {
      return;
    }
    if (!isset($this->rendered_fields)) {
      $this->rendered_fields = array();
      $field_ids = array_keys($this->view->field);
      foreach ($result as $i => $row) {
        $this->view->row_index = $i; // God knows why we need this...
        foreach ($field_ids as $field_id) {
          // add the field label if it's provided
          $label = $this->view->field[$field_id]->label();
          $element = '';
          $close_element = '';
          if ($label) {
            $label_type = $this->view->field[$field_id]->options['element_label_type'];
            if ($label_type) {
              $label_type = check_plain($label_type);
              $element = '<' . $label_type;
              $label_class = $this->view->field[$field_id]->options['element_label_class'];
              if ($label_class) {
                $element .= ' class="' . check_plain($label_class) . '"';
              }
              $element .= '>';
              $close_element = '</' . $label_type . '>';
            }
            if ($this->view->field[$field_id]->options['element_label_colon']) {
              $label .= ': ';
            }
            $this->rendered_fields[$i][$field_id] = $element . $label . $close_element . ' ' . $this->view->field[$field_id]->theme($row);
          } // otherwise render with no label
          else {
            $this->rendered_fields[$i][$field_id] = $this->view->field[$field_id]->theme($row);
          }
        }
        //$this->row_tokens[$i] = $this->view->field[$field_id]->get_render_tokens(array());
      }
      unset($this->view->row_index);
    }
    return $this->rendered_fields;
  }

}