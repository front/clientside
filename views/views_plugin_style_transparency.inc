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
//    $options['']  = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    xdebug_break();
    $form['add_display'] = array(
    '#title' => t('Add displays'),
    '#type' => 'select',
    '#options' => array('test1' => 'Json View1', 'test2' => 'Json view 2', 'test3' => 'Json view 3', 'test4' => 'Json view 4'),
    '#default_value' => $form_state['values']['add_display'],
    '#ajax' => array(
      'callback' => 'transparency_view_options',
      'wrapper' => 'displays-used',
      'method' => 'replace',
      'effect' => 'fade',
    )
   );
    $form['main_fieldset'] = array(
    '#title' => t("Data"),
    '#prefix' => '<div id="displays-used">',
    '#suffix' => '</div>',
    '#type' => 'fieldset',
    '#description' => t('Please select.'),
   );
    if (!empty($form_state['values']['add_display'])){
    if (!isset($form['main_fieldset']['display_type_' . $form_state['values']['add_display']])){
      $form['main_fieldset'][($form['display_type_' . $form_state['values']['add_display']])] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#default_value' => $form_state['values']['add_display'],
      );
      $form[($form['main_fieldset']['display_type_' . $form_state['values']['add_display']]) . '_mappings'] = array(
        '#tree' => 'true',
        '#type' => 'fieldset',
        '#title' => 'Field mappings',
        '#collapsible' => true,
      );
      $template_fields = array(
        'field_1' => t('Template field 1'),
        'field_2'=> t('Template field 2'),
      );
      $fields = array(
        'field_1' => t('Original field 1'),
        'field_2'=> t('Original field 2'),
      );
      foreach($template_fields AS $fid => $fname){
        $form['main_fieldset'][($form['display_type_' . $form_state['values']['add_display']]) . '_mappings']['display_' . $fid] = array(
        '#type' =>'select',
        '#title' => t('Map template field %field to...', array('%field' => $fname)),
          '#options' => $fields,
      );
      $form['main_fieldset'][($form['display_type_' . $form_state['values']['add_display']]) . '_mappings']['delete_this'] = array(
        '#type' => 'submit',
        '#value' => t('Delete this entry'),
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
    } else {

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