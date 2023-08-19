#include <ArduinoEigen.h>
#include <ArduinoEigenDense.h>
#include <ArduinoEigenSparse.h>


#include <Slerp.h>
#include <Quaternion.h>

Eigen::Matrix3d euler2rotm(const Eigen::Vector3d& euler_angs) {
    double roll = euler_angs[0] * PI / 180.0;
    double pitch = euler_angs[1] * PI / 180.0;
    double yaw = euler_angs[2] * PI / 180.0;

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

Eigen::MatrixX3d interpolate_rpy(const Eigen::Vector3d& start_rpy, const Eigen::Vector3d& end_rpy, int num_points) {
    Eigen::Vector3d start_end[2] = { start_rpy, end_rpy };

    // Create Slerp object and perform interpolation
    Quaterniond q_start_end[2];
    for (int i = 0; i < 2; ++i) {
        AngleAxisd angle_axis(start_end[i][0] * PI / 180.0, Vector3d::UnitX());
        angle_axis *= AngleAxisd(start_end[i][1] * PI / 180.0, Vector3d::UnitY());
        angle_axis *= AngleAxisd(start_end[i][2] * PI / 180.0, Vector3d::UnitZ());
        q_start_end[i] = angle_axis;
    }

    Quaterniond q_interp;
    Eigen::MatrixX3d interpolated_rotations(num_points, 3);
    for (int i = 0; i < num_points; ++i) {
        double t = static_cast<double>(i) / (num_points - 1);
        q_interp = q_start_end[0].slerp(t, q_start_end[1]);
        Eigen::Vector3d eul_interp = q_interp.toRotationMatrix().eulerAngles(0, 1, 2) * 180.0 / PI;
        interpolated_rotations.row(i) = eul_interp;
    }

    return interpolated_rotations;
}

Eigen::Vector3d z_ax(0, 0, 1);
Eigen::Vector3d y_ax(0, 1, 0);
Eigen::Vector3d x_ax(1, 0, 0);

Eigen::Vector3d rpy(45, 120, 270);

Eigen::Vector3d start_rpy(0, 0, 0);
Eigen::Vector3d end_rpy = rpy;
int num_points = 10;

Eigen::MatrixX3d interp_rot = interpolate_rpy(start_rpy, end_rpy, num_points);

std::vector<Eigen::Vector3d> euls_list;
euls_list.push_back(x_ax);
euls_list.push_back(y_ax);
euls_list.push_back(z_ax);

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        // Wait for Serial connection
    }
}

void loop() {
    for (int i = 0; i < num_points; ++i) {
        Eigen::Matrix3d rotation_matrix = euler2rotm(interp_rot.row(i));
        Eigen::Vector3d rotated_vector = rotation_matrix * z_ax;
        euls_list.push_back(rotated_vector);

        // Print rotated vector values
        Serial.print("Rotated Vector (");
        Serial.print(rotated_vector[0]);
        Serial.print(", ");
        Serial.print(rotated_vector[1]);
        Serial.print(", ");
        Serial.print(rotated_vector[2]);
        Serial.println(")");
    }

    // Stop further execution
    while (1) {}
}
