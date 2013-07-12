<?php

/**
 * @file
 * Administrative forms section
 */

/**
 * Administration form for RingCaptcha settings.
 */
function ringcaptcha_admin_form($form, &$form_state) {

  // Detect the RingCaptcha library and provide feedback to the user if not present.
  if (module_exists('libraries') && function_exists('libraries_detect')) {
      $library = libraries_detect(RINGCAPTCHA_LIBRARY);
      if (!$library['installed']) {
        drupal_set_message(t('Ringcaptcha library was not detected or installed correctly.') . ' ' . l(t('Download the library here.'), 'https://bitbucket.org/ringcaptcha/ringcaptcha-php')
          , 'error');
        drupal_set_message(t('If drush is installed in your server you can execute the command "drush ringcaptcha-download" to easily download the library.'), 'status');
      }
  }
  
  $form['ringcaptcha_appkey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('RingCaptcha App key'),
    '#default_value' => variable_get('ringcaptcha_appkey'),
    '#description' => t('Enter your RingCaptcha App key'),
  );
  $form['ringcaptcha_secretkey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('RingCaptcha Secret key'),
    '#default_value' => variable_get('ringcaptcha_secretkey'),
    '#description' => t('Enter your RingCaptcha Secret key'),
  );
  $form['ringcaptcha_require_ssl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send the request using SSL'),
    '#description' => t('Configure whether you want to send the request using SSL, we suggest always connecting via SSL'),
    '#default_value' => variable_get('ringcaptcha_require_ssl'),
  );
  $form['ringcaptcha_registration_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show RingCaptcha in the user registration'),
    '#description' => t('Specify if the site should verify with mobile information during registration.'),
    '#default_value' => variable_get('ringcaptcha_registration_form'),
  );

  return system_settings_form($form);
}
