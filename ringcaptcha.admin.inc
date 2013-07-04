<?php

/**
 * @file
 * Administrative forms section
 */

/**
 * Administration form for RingCapctha settings.
 */
function ringcaptcha_admin_form($form, &$form_state) {
    
  // Detect the RingCaptcha library and provide feedback to the user if not present.
  if (module_exists('libraries') && function_exists('libraries_detect')) {
      $library = libraries_detect(RINGCAPTCHA_LIBRARY);
      if (!$library['installed']) {
        $ringcaptcha_readme_link = l('Documentation & Download link', 'https://bitbucket.org/ringcaptcha/ringcaptcha-php' );
        $ringcaptcha_error_text = t('The RingCaptcha library was not detected or installed correctly. Please download the library provided in the !link', array('!link' => $ringcaptcha_readme_link));
        drupal_set_message($ringcaptcha_error_text, 'error');
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
    '#title' => t('Send the request using SSL, make sure your website is configure to handle or have SSL certificate'),
    '#description' => t('Configure whether you want to send the request using SSL or not. We suggest always connecting via SSL'),
    '#default_value' => variable_get('ringcaptcha_require_ssl', 0),
  );
  $form['ringcaptcha_registration_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show RingCaptcha during user registration'),
    '#description' => t('Specify if the site should verify with mobile information during registration.'),
    '#default_value' => variable_get('ringcaptcha_registration_form', 0),
  );
  return system_settings_form($form);
  
}
