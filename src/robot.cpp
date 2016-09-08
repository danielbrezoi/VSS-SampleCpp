#include "robot.h"

Robot::Robot(){
	id = -1;
    robot_side_size = 8.0;
    max_velocity_wheel = 1000.0;
    max_aceleration_wheel = 1000.0;
    angle_to_spin = 135.0;

    need_brutal_mode = false;
    need_to_replan_path = true;    
    front_and_rear_movimentaion = false;

	task = Task::GOAL_KEEPER;
}

void Robot::calc_action(){
	switch(task){
		case Task::GOAL_KEEPER:{
			GK_calc_action();
		}break;
		case Task::DEFENDER:{
			DF_calc_action();
		}break;
		case Task::ATTACKER:{
			AT_calc_action();
		}break;
	}
}

// MODEL BLAZIC
void Robot::calc_cmd_to(){
    float distance_robot_goal;
	float angulation_robot_goal;
	float angulation_robot_robot_goal;

	// Diferença entre angulação do robô e do objetivo
	distance_robot_goal = distancePoint(step_pose, pose);
	angulation_robot_goal = angulation(step_pose, pose);
	//step_pose.show();
	angulation_robot_goal -= 180; 
    if(angulation_robot_goal < 0){
    	angulation_robot_goal += 360;
    }

	angulation_robot_robot_goal = pose.z - angulation_robot_goal;

	if(angulation_robot_robot_goal > 180){
		angulation_robot_robot_goal -= 360;
	}

	if(angulation_robot_robot_goal < -180){
		angulation_robot_robot_goal += 360;
	}
	
	// Regras de movimentação
	if(fabs(angulation_robot_robot_goal) <= angle_to_spin){
		cmd.left = distance_robot_goal - 0.2*(angulation_robot_robot_goal * robot_side_size / 2.00);
		cmd.right = distance_robot_goal + 0.2*(angulation_robot_robot_goal * robot_side_size / 2.00);
		
		cmd.left *= 0.3;
		cmd.right *= 0.3;
	}else{
        // SPIN
		if(angulation_robot_robot_goal >= 0){
			cmd.left = 50;
			cmd.right = -50;
		}else{
			cmd.left = -50;
			cmd.right = 50;
		}
	}

	if(distance_robot_goal < 15.0){
		cmd.left = 0;
		cmd.right = 0;
	}
}

void Robot::alloc_our_team(vector<Robot> *our_team){
    this->our_team = our_team;
}

void Robot::alloc_adversary_team(vector<Robot> *adversary_team){
    this->adversary_team = adversary_team;
}

void Robot::alloc_ball(btVector3 *ball, btVector3 *v_ball){
    this->ball = ball;
    this->v_ball = v_ball;
}

void Robot::set_id(int id){
	this->id = id;
	switch(id){
		case 0:{
			task = Task::GOAL_KEEPER;
		}break;
		case 1:{
			task = Task::DEFENDER;
		}break;
		case 2:{
			task = Task::ATTACKER;
		}break;
	}
}

void Robot::set_task(Task task){
	this->task = task;
}

void Robot::set_pose(btVector3 pose){
    this->pose = pose;
}

void Robot::set_v_pose(btVector3 v_pose){
    this->v_pose = v_pose;
}

btVector3 Robot::get_final_pose(){
    return final_pose;
}

btVector3 Robot::get_step_pose(){
    return step_pose;
}

Path Robot::get_path(){
    return path;
}

Command Robot::get_command(){
    return cmd;
}