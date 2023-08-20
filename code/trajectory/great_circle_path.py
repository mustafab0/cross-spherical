import numpy as np
import matplotlib.pyplot as plt
np.set_printoptions(precision=4)

def plot_vector(unit_vectors):
    # Create a figure and a 3D subplot
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Create spherical coordinates
    u = np.linspace(0, 2 * np.pi, 20)
    v = np.linspace(0, np.pi, 20)
    u, v = np.meshgrid(u, v)

    # Convert spherical coordinates to Cartesian coordinates for unit sphere
    x = np.cos(u) * np.sin(v)
    y = np.sin(u) * np.sin(v)
    z = np.cos(v)

    # Plot the unit sphere
    ax.plot_surface(x, y, z, color = 'b', alpha=0.2)
    
    #plot meshgrid
    ax.plot_wireframe(x, y, z, color = 'k', alpha=0.1)

    # Define a list of colors
    colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k']
    # Plot unit vectors
    for i, unit_vector in enumerate(unit_vectors):
        # Normalize the vector to make it a unit vector
        unit_vector = unit_vector / np.linalg.norm(unit_vector)
        
        # Plot the unit vector as a quiver with a different color
        color = colors[i % len(colors)]  # Cycle through colors
        ax.quiver(0, 0, 0, unit_vector[0], unit_vector[1], unit_vector[2],color=color)

    # Plot the origin
    ax.plot([0], [0], [0], marker='+', color='r', label='Origin')

    # Set axis labels
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Set plot title
    ax.set_title('Unit Sphere')
    ax.axis('equal')

    # Enable interactive rotation, panning, and zooming
    ax.view_init(elev=20, azim=30)
    ax.mouse_init()

    # Show the plot
    plt.show()

    return ax

def euler2rotm(euler_angs):
    roll = np.deg2rad(euler_angs[0])
    pitch = np.deg2rad(euler_angs[1])
    yaw = np.deg2rad(euler_angs[2])
    R_x = np.array([[1, 0, 0],
                    [0, np.cos(roll), -np.sin(roll)],
                    [0, np.sin(roll), np.cos(roll)]
                    ])

    R_y = np.array([[np.cos(pitch), 0, np.sin(pitch)],
                    [0, 1, 0],
                    [-np.sin(pitch), 0, np.cos(pitch)]
                    ])

    R_z = np.array([[np.cos(yaw), -np.sin(yaw), 0],
                    [np.sin(yaw), np.cos(yaw), 0],
                    [0, 0, 1]
                    ])

    R = R_z @ R_y @ R_x

    return R


class Quaternion:
    def __init__(self, x, y, z, w):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

def to_quaternion(rpy):
    # rpy = np.deg2rad(rpy)
    roll = np.deg2rad(rpy.roll)
    pitch = np.deg2rad(rpy.pitch)
    yaw = np.deg2rad(rpy.yaw)

    cr = np.cos(roll * 0.5)
    sr = np.sin(roll * 0.5)
    cp = np.cos(pitch * 0.5)
    sp = np.sin(pitch * 0.5)
    cy = np.cos(yaw * 0.5)
    sy = np.sin(yaw * 0.5)

    q = Quaternion(
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy,
        cr * cp * cy + sr * sp * sy
    )

    return q

class EulerAngles:
    def __init__(self, roll, pitch, yaw):
        self.roll = roll
        self.pitch = pitch
        self.yaw = yaw

def to_euler_angles(q):
    angles = EulerAngles(0,0,0)

    # roll (x-axis rotation)
    sinr_cosp = 2 * (q.w * q.x + q.y * q.z)
    cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y)
    angles.roll = np.arctan2(sinr_cosp, cosr_cosp)

    # pitch (y-axis rotation)
    sinp = np.sqrt(1 + 2 * (q.w * q.y - q.x * q.z))
    cosp = np.sqrt(1 - 2 * (q.w * q.y - q.x * q.z))
    angles.pitch = 2 * np.arctan2(sinp, cosp) - np.pi / 2

    # yaw (z-axis rotation)
    siny_cosp = 2 * (q.w * q.z + q.x * q.y)
    cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z)
    angles.yaw = np.arctan2(siny_cosp, cosy_cosp)

    # Convert to degrees
    angles.roll = np.rad2deg(angles.roll)
    angles.pitch = np.rad2deg(angles.pitch)
    angles.yaw = np.rad2deg(angles.yaw)

    return angles

def quaternion_slerp(qstart, qend, t):
    # Compute the dot product between the two quaternions
    q0 = np.array([qstart.x, qstart.y, qstart.z, qstart.w])
    q1 = np.array([qend.x, qend.y, qend.z, qend.w])

    dot = np.dot(q0, q1)

    
    DOT_THRESHOLD = 0.9995
    if dot > DOT_THRESHOLD:
        # Linearly interpolate and normalize if inputs are too close
        result = q0 + t * (q1 - q0)
        result /= np.linalg.norm(result)
        return result
    

    # Clamp the dot product to stay within the domain of acos()
    dot = np.clip(dot, -1, 1)

    # Compute theta_0 and theta
    theta_0 = np.arccos(dot)
    theta = theta_0 * t
    
    # Compute intermediate quaternion v2
    v2 = q1 - q0 * dot
    v2 /= np.linalg.norm(v2)
    
    # Slerp interpolation
    result = q0 * np.cos(theta) + v2 * np.sin(theta)

    q_result = Quaternion(result[0], result[1], result[2], result[3])
    
    return q_result

def q_mult(q1, q2):
    # w1, x1, y1, z1 = q1
    # w2, x2, y2, z2 = q2
    x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y
    y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z
    z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x
    w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z

    q_result = Quaternion(x,y,z,w)
    
    #normalize quaternion
    q_result_norm = np.sqrt(q_result.x**2 + q_result.y**2 + q_result.z**2 + q_result.w**2)
    q_result.x = q_result.x / q_result_norm
    q_result.y = q_result.y / q_result_norm
    q_result.z = q_result.z / q_result_norm
    q_result.w = q_result.w / q_result_norm

    return q_result

z_ax = np.array([0, 0, 1])
y_ax = np.array([0, 1, 0])
x_ax = np.array([1, 0, 0]) 

rpy = np.array([80, 150, 50])  #destination RPY angles  ###Change values here###
plt_vec = z_ax #initial vector for plot

# Define start and end RPY angles (in degrees)
start_rpy = EulerAngles(0, 0, 0)
# end_rpy = EulerAngles(rpy[0], rpy[1], rpy[2])

cont = 'y'

while cont == 'y':
    rpy = np.array([float(input("Enter roll: ")), float(input("Enter pitch: ")), float(input("Enter yaw: "))])
    print("input rpy: ", rpy)
    end_rpy = EulerAngles(rpy[0], rpy[1], rpy[2])
    # end_rpy = EulerAngles(start_rpy.roll + rpy[0], start_rpy.pitch + rpy[1], start_rpy.yaw + rpy[2])
    start_quat = to_quaternion(start_rpy)
    end_quat = to_quaternion(end_rpy)
    end_quat = q_mult(end_quat, start_quat)
    end_rpy = to_euler_angles(end_quat)

    #print start and end euler angles
    print("Start RPY angles: ", start_rpy.roll, start_rpy.pitch, start_rpy.yaw)
    print("End RPY angles: ", end_rpy.roll, end_rpy.pitch, end_rpy.yaw)

    # #print start and end quaternions
    # print("Start Quaternion: ", start_quat.x, start_quat.y, start_quat.z, start_quat.w)
    # print("End Quaternion: ", end_quat.x, end_quat.y, end_quat.z, end_quat.w)

    euls_list = [x_ax, y_ax, z_ax]
    rpy_list = []
    # Perform Slerp interpolation for different values of t
    num_points = 10
    for i in range(num_points + 1):
        t = i / num_points
        interpolated_quat = quaternion_slerp(start_quat, end_quat, t)
        interpolated_rpy = to_euler_angles(interpolated_quat)
        rpy_list.append(interpolated_rpy)
        euls_list.append(euler2rotm(np.array([interpolated_rpy.roll , interpolated_rpy.pitch , interpolated_rpy.yaw]))@z_ax)

    print("Vector list: ")
    for vec in euls_list:
        print("[",vec[0],",",vec[1],",",vec[2],"]",",")
    
    print("RPY angles: ")
    for euls in rpy_list:
        print("[",euls.roll,",",euls.pitch,",",euls.yaw,"]",",")
    
    start_rpy = end_rpy
    plt_vec = euls_list[-1]

    print(plt_vec)
    ax = plot_vector(euls_list)
    cont = input("Continue? (y/n): ")
    

    
        
    