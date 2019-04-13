

void setupRTOS(){
  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.

  if ( xSDSemaphoreMR == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
  {
    /* Create a recursivemutex semaphore without using any dynamic
      memory allocation.  The mutex's data structures will be saved into
      the xMutexBuffer variable. */
    xSDSemaphoreMR = xSemaphoreCreateRecursiveMutex( );  // Create a mutex semaphore we will use to manage the Serial Port

  }

}
