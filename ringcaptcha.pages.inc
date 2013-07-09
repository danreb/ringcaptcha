<?php

/**
 * @file
 * User facing pages
 */

/**
 * Menu callback; provides the forms for user's mobile number.
 */
function ringcaptcha_user_settings($account) {
  return drupal_get_form('ringcaptcha_user_settings_form', $account);
}

/**
 * User settings form.
 */
function ringcaptcha_user_settings_form($form, &$form_state, $account) {

  if (isset($form_state['values']['account'])) {
    $account = user_load($form_state['values']['account']->uid);
  }

  $form['#prefix'] = '<div id="ringcaptcha-user-form">';
  $form['#suffix'] = '</div>';

  $form['uid'] = array(
      '#type' => 'hidden',
      '#value' => $account->uid,
  );

  if (empty($account->ringcaptcha_user['status'])) {
    $form['number'] = array(
        '#type' => 'textfield',
        '#title' => t('Mobile Phone number'),
        '#description' => t('You need to confirm your Mobile Phone number via RingCaptcha, confirmation code will be sent to your number via SMS'),
        '#size' => 40,
        '#maxlength' => 255,
        '#required' => TRUE,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update number'),
        '#validate' => array('ringcaptcha_user_settings_add_form_validate'),
        '#submit' => array('ringcaptcha_user_settings_add_form_submit'),
        '#ajax' => array(
            'callback' => 'ringcaptcha_user_settings_ajax_callback',
            'wrapper' => 'ringcaptcha-user-form',
            'method' => 'replace',
        )
    );
  }
  elseif ($account->ringcaptcha_user['status'] == 1) {
    $form['number'] = array(
        '#type' => 'item',
        '#title' => t('Mobile phone number'),
        '#markup' => $account->ringcaptcha_user['number'],
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Confirm number'),
        '#validate' => array('ringcaptcha_user_settings_confirm_form_validate'),
        '#submit' => array('ringcaptcha_user_settings_confirm_form_submit'),
        '#ajax' => array(
            'callback' => 'ringcaptcha_user_settings_ajax_callback',
            'wrapper' => 'ringcaptcha-user-form',
            'method' => 'replace',
        )
    );
    $form['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Delete & start over'),
        '#validate' => array('ringcaptcha_user_settings_confirm_form_validate'),
        '#submit' => array('ringcaptcha_user_settings_confirm_form_submit'),
        '#ajax' => array(
            'callback' => 'ringcaptcha_user_settings_ajax_callback',
            'wrapper' => 'ringcaptcha-user-form',
            'method' => 'replace',
        )
    );
  }
  elseif ($account->ringcaptcha_user['status'] == 2) {
    $form['ringcaptcha_user']['number'] = array(
        '#type' => 'item',
        '#title' => t('Your mobile phone number'),
        '#markup' => $account->ringcaptcha_user['number'],
        '#description' => t('Your mobile phone number has been confirmed.'),
    );
    $form['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Delete & start over'),
        '#validate' => array('ringcaptcha_user_settings_reset_form_validate'),
        '#submit' => array('ringcaptcha_user_settings_reset_form_submit'),
        '#ajax' => array(
            'callback' => 'ringcaptcha_user_settings_ajax_callback',
            'wrapper' => 'ringcaptcha-user-form',
            'method' => 'replace',
        )
    );
  }
  return $form;
}

/**
 * Ajax callback for user forms.
 */
function ringcaptcha_user_settings_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validation function for user settings form.
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ringcaptcha_user_settings_add_form_validate($form, &$form_state) {
  if (ringcaptcha_verify_duplicate_number($form_state['values']['number'])) {
    form_set_error('number', t('This number is already in use and cannot be assigned to more than one account'));
  }
}

/**
 * Submit handler for user settings form.
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ringcaptcha_user_settings_add_form_submit($form, &$form_state, $account = NULL) {
  if (!$account) {
    $account = user_load($form_state['values']['uid']);
  }
  $account = ringcaptcha_user_send_confirmation($account, $form_state['values']['number']);
  $form_state['values']['account'] = $account;
  $form_state['rebuild'] = TRUE;
  drupal_set_message(t("A confirmation code has been sent to your mobile device"), 'status');
}

/**
 * Validation handler for user settings confirmation form.
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ringcaptcha_user_settings_confirm_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Confirm number')) {
    $account = user_load($form_state['values']['uid']);
    if ($form_state['values']['confirm_code'] != $account->ringcaptcha_user['code']) {
      form_set_error('confirm_code', t('The confirmation code is invalid.'));
    }
  }
}

/**
 * Submit handler for user settings confirmation form.
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ringcaptcha_user_settings_confirm_form_submit($form, &$form_state) {
  $account = user_load($form_state['values']['uid']);
  if ($form_state['clicked_button']['#value'] == t('Delete & start over')) {
    ringcaptcha_user_delete($account);
    $account = user_load($form_state['values']['uid'], TRUE);
    drupal_set_message(t('Your mobile information has been removed'), 'status');
  }
  else {
    $data = array(
        'number' => $account->twilio_user['number'],
        'status' => RINGCAPTCHA_USER_CONFIRMED,
    );
    $account = user_save($account, array('ringcaptcha' => $data), 'ringcaptcha');
    drupal_set_message(t('Your mobile number has been confirmed'), 'status');
  }
  $form_state['values']['account'] = $account;
  $form_state['rebuild'] = TRUE;
}

/**
 * Validation handler for user settings reset form.
 */
function ringcaptcha_user_settings_reset_form_validate($form, &$form_state) {
}

/**
 * Submit handler for user settings reset form.
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ringcaptcha_user_settings_reset_form_submit($form, &$form_state) {
  $account = user_load($form_state['values']['uid']);
  ringcaptcha_user_delete($account);
  $account = user_load($form_state['values']['uid'], TRUE);
  drupal_set_message(t('Your mobile information has been removed'), 'status');
  $form_state['values']['account'] = $account;
  $form_state['rebuild'] = TRUE;
}

