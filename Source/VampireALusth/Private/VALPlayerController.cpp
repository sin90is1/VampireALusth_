// Fill out your copyright notice in the Description page of Project Settings.


#include "VALPlayerController.h"


// void AVALPlayerController::SetupInputComponent()
// {
// 	 InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
// 	 InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
// 	 
// 	 
// 	 InputComponent->BindAxis("Move Forward / Backward", this, &AVampireALusthCharacter::MoveForward);
// 	 InputComponent->BindAxis("Move Right / Left", this, &AVampireALusthCharacter::MoveRight);
// 	 
// 	 	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
// 	 	// "turn" handles devices that provide an absolute delta, such as a mouse.
// 	 	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
// 	 InputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
// 	 InputComponent->BindAxis("Turn Right / Left Gamepad", this, &AVampireALusthCharacter::TurnAtRate);
// 	 InputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
// 	 InputComponent->BindAxis("Look Up / Down Gamepad", this, &AVampireALusthCharacter::LookUpAtRate);
// 	 
// 	 	// handle touch devices
// 	 InputComponent->BindTouch(IE_Pressed, this, &AVampireALusthCharacter::TouchStarted);
// 	 InputComponent->BindTouch(IE_Released, this, &AVampireALusthCharacter::TouchStopped);
// 	 
// 	 InputComponent->BindAction("Sprints", IE_Pressed, this, &AVampireALusthCharacter::Sprint);
// 	 InputComponent->BindAction("Sprints", IE_Released, this, &AVampireALusthCharacter::StopSprint);
// 	 
// 	 InputComponent->BindAction("Crouch", IE_Pressed, this, &AVampireALusthCharacter::StartCrouch);
//      InputComponent->BindAction("Crouch", IE_Released, this, &AVampireALusthCharacter::StopCrouch);
// /*	 GetCharacterMovement()->NavAgentProps.bCanCrouch = true;*/
// }