/*
 * File: mouse_single_hind.c
 * Date: 23.03.2017
 * Description: Physics plugin to fix the spine of the complete mouse model to the world
 * with a linear slider joint in Y and Z direction to simulate support 
 * Author: Shravan Tata Ramalingasetty
 * Modifications:
 */

#include <ode/ode.h>
#include <plugins/physics.h>


static dBodyID body_Spine;
static dBodyID body_Reference;
static dBodyID body_Support;

static dJointID joint_Fixed;
static dJointID joint_Rotate_X;
static dJointID joint_Slider_Y;

void webots_physics_step() {

}

void webots_physics_init()
{
	dWebotsConsolePrintf("Mouse hind limb with support physic plug-in initiated\n");

	//find body in the scene tree
	body_Spine = dWebotsGetBodyFromDEF("spine");
	if (body_Spine == NULL)
	{	//we check we found the body
		dWebotsConsolePrintf("ERROR: could not find the body_Spine \n    In physic plug-in.\n");
		return;
	}

	dWorldID world = dBodyGetWorld(body_Spine);
	body_Reference = dBodyCreate (world); // Body to fix to the world frame
	body_Support = dBodyCreate (world); // Body to fix to the world frame

	// Create a fixed body attached to the world

	joint_Fixed = dJointCreateFixed(world, 0); // Create a fixed joint with the world

	dJointAttach (joint_Fixed, body_Reference, 0); // Fix the reference body to the world

	dJointSetFixed (joint_Fixed); // Initialize the joint


	// Create a Rotation joint in X direction for lateral movement support between the reference body and the spine

	joint_Rotate_X = dJointCreateHinge(world,0); // Create a slider joint

	dJointAttach (joint_Rotate_X, body_Reference, body_Support); // Attach joint between reference and pelvis

	dJointSetHingeAxis(joint_Rotate_X, 1 ,0, 0); // Create a slider along the z-axis

	// Create a prismatic-rotaide joint in Y direction for height support between the reference and the spine

	joint_Slider_Y = dJointCreateSlider(world,0); // Create a slider joint

	dJointAttach (joint_Slider_Y, body_Support, body_Spine); // Attach joint between reference and pelvis

	dJointSetSliderAxis(joint_Slider_Y, 0 ,1, 0); // Create a slider along the z-axis

}

int webots_physics_collide(dGeomID g1, dGeomID g2) {
	/*
	 * This function needs to be implemented if you want to overide Webots collision detection.
	 * It must return 1 if the collision was handled and 0 otherwise.
	 * Note that contact joints should be added to the contactJointGroup, e.g.
	 *   n = dCollide(g1, g2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
	 *   ...
	 *   dJointCreateContact(world, contactJointGroup, &contact[i])
	 *   dJointAttach(contactJoint, body1, body2);
	 *   ...
	 */
	return 0;
}

void webots_physics_cleanup() {
	/*
	 * Here you need to free any memory you allocated in above, close files, etc.
	 * You do not need to free any ODE object, they will be freed by Webots.
	 */
}
