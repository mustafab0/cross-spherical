#include <ArduinoEigen.h>
#include <ArduinoEigenDense.h>
#include <ArduinoEigenSparse.h>

#define DEG2RAD 0.0174532925
#define RAD2DEG 57.2957795

Eigen::Matrix3d euler2rotm(const Eigen::Vector3d& euler_angs) {
    double roll = euler_angs(0) * DEG2RAD;
    double pitch = euler_angs(1) * DEG2RAD;
    double yaw = euler_angs(2) * DEG2RAD;
    Eigen::Matrix3d R_x;
    R_x << 1, 0, 0,
           0, cos(roll), -sin(roll),
           0, sin(roll), cos(roll);

    Eigen::Matrix3d R_y;
    R_y << cos(pitch), 0, sin(pitch),
           0, 1, 0,
           -sin(pitch), 0, cos(pitch);

    Eigen::Matrix3d R_z;
    R_z << cos(yaw), -sin(yaw), 0,
           sin(yaw), cos(yaw), 0,
           0, 0, 1;

    Eigen::Matrix3d R = R_z * R_y * R_x;

    return R;
}

struct Quaternion {
    double x;
    double y;
    double z;
    double w;
};

struct Quaternion to_quaternion(const Eigen::Vector3d& rpy) {

    double roll  = rpy(0) * DEG2RAD;
    double pitch = rpy(1) * DEG2RAD;
    double yaw   = rpy(2) * DEG2RAD;

    double cr = std::cos(roll * 0.5);
    double sr = std::sin(roll * 0.5);
    double cp = std::cos(pitch * 0.5);
    double sp = std::sin(pitch * 0.5);
    double cy = std::cos(yaw * 0.5);
    double sy = std::sin(yaw * 0.5);

    struct Quaternion q = {
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    };

    return q;
}

Eigen::Vector3d to_euler_angles(const struct Quaternion& q) {
    Eigen::Vector3d angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles(0) = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles(1) = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles(2) = std::atan2(siny_cosp, cosy_cosp);

    // Convert to degrees
    angles(0) = angles(0) * RAD2DEG;
    angles(1) = angles(1) * RAD2DEG;
    angles(2) = angles(2)* RAD2DEG;

    return angles;
}

struct Quaternion q_slerp(const struct Quaternion& qstart, const struct Quaternion& qend, double t)
{
	// Compute the dot product between the two quaternions
	Eigen::Vector4d q0(qstart.x, qstart.y, qstart.z, qstart.w);
	Eigen::Vector4d q1(qend.x, qend.y, qend.z, qend.w);
	
	double dot = q0.transpose() * q1;
	
	const double DOT_THRESHOLD = 0.9995;
	if (dot > DOT_THRESHOLD)
	{
		// Linearly interpolate and normalize if inputs are too close
		Eigen::Vector4d result = q0 + t * (q1 - q0);
		result /= result.norm();
    struct Quaternion q_result= {
        result(0),
        result(1),
        result(2),
        result(3)
    };
		return q_result;
	}
	
	// Clamp the dot product to stay within the domain of acos()
	dot = std::max(-1.0, std::min(1.0, dot));

	// Compute theta_0 and theta
	double theta_0 = std::acos(dot);
	double theta = theta_0 * t;

 	// Compute intermediate quaternion v2
	Eigen::Vector4d v2 = q1 - q0 * dot;
	v2 /= v2.norm();
	
	// Slerp interpolation
	Eigen::Vector4d result = q0 * std::cos(theta) + v2 * std::sin(theta);

  struct Quaternion q_result= {
        result(0),
        result(1),
        result(2),
        result(3)
    };
	
	return q_result;
}

struct Quaternion q_mult(const struct Quaternion& q1, const struct Quaternion& q2)
{
	struct Quaternion q_result;
	q_result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	q_result.y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
	q_result.z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;
	q_result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

	//normalize quaternion
	float q_result_norm = sqrt(q_result.x*q_result.x + q_result.y*q_result.y + q_result.z*q_result.z + q_result.w*q_result.w);
	q_result.x = q_result.x / q_result_norm;
	q_result.y = q_result.y / q_result_norm;
	q_result.z = q_result.z / q_result_norm;
	q_result.w = q_result.w / q_result_norm;

	return q_result;
}


void get_euler_angles(int i, Eigen::Vector3d &end_rpy) {

  Serial.println("Enter euler angles (roll, pitch, yaw):");

  switch (i) {
      case 0:
          Serial.print("Roll: ");
          break;
      case 1:
          Serial.print("Pitch: ");
          break;
      case 2:
          Serial.print("Yaw: ");
          break;
      default:
          break;
    }
  //Clear the input buffer
  while (Serial.available()) {
      Serial.read();
  }
  while (!Serial.available());
  end_rpy(i) = Serial.parseFloat();
  Serial.println(end_rpy(i));

}

void print_euler_angles(Eigen::Vector3d eul_ang) {
    Serial.print(eul_ang(0));
    Serial.print(", ");
    Serial.print(eul_ang(1));
    Serial.print(", ");
    Serial.print(eul_ang(2));
  }

void traj_plan(Eigen::Vector3d &start_rpy, Eigen::Vector3d &end_rpy, std::vector<Eigen::Vector3d> &vec_list)
  {
      struct Quaternion start_quat, end_quat;
      start_quat = to_quaternion(start_rpy);
      end_quat = to_quaternion(end_rpy);
      end_quat = q_mult(end_quat, start_quat);
      end_rpy = to_euler_angles(end_quat);

      Serial.print("Start euler angles: ");
      print_euler_angles(start_rpy);
      Serial.print("\n");

      Serial.print("End euler angles: ");
      print_euler_angles(end_rpy);
      Serial.print("\n");

      struct Quaternion interpolated_quat;
      Eigen::Vector3d interpolated_rpy;
      
      int num_points = 10;
      for (int i = 0; i <= num_points; i++)
      {
          double t = i / (double) num_points;
          interpolated_quat = q_slerp(start_quat, end_quat, t);
          interpolated_rpy = to_euler_angles(interpolated_quat);
          // vec_list.push_back(euler2rotm(interpolated_rpy)*p_vec);   //Uncomment to record the pointer for graphing
          vec_list.push_back(interpolated_rpy);                      //Uncomment to record RPY angles for Inverse Kinematics 
          
      }
      start_rpy = end_rpy;
    }

Eigen::Vector3d start_rpy(0, 0, 0);
Eigen::Vector3d p_vec(0,0,1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  delay(500);

  Serial.println("Trajectory planning");
  
}

void loop() {

  std::vector<Eigen::Vector3d> vec_list;
  Eigen::Vector3d end_rpy;

  for(int i = 0; i < 3; i++){
    get_euler_angles(i, end_rpy);
  }
  
  traj_plan(start_rpy, end_rpy, vec_list);

  // Print out the list of vectors
  for (int i = 0; i < (int) vec_list.size(); i++)
  {
    Serial.print("[");
    print_euler_angles(vec_list[i]);
    Serial.println("],");
  }

  // Ask if want to continue
  Serial.println("Continue? (y/n)");
  //Clear the input buffer
  while (Serial.available()) {
      Serial.read();
  }
  while (!Serial.available());
  char c = Serial.read();

  if (c != 'y'){
    Serial.print("End");
    while(1){} // Wait forever
  }
  else{
    vec_list.clear();
  }
    
}
