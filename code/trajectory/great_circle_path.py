# #plot a great circle path on a map

# import numpy as np
# import matplotlib.pyplot as plt 

# # define the great circle path
# def great_circle_path(lat1, lon1, lat2, lon2, npts):
#     # convert to radians
#     lat1 = np.radians(lat1)
#     lon1 = np.radians(lon1)
#     lat2 = np.radians(lat2)
#     lon2 = np.radians(lon2)
    
#     # calculate the angle between the two points
#     d = 2*np.arcsin(np.sqrt(np.sin((lat2-lat1)/2)**2 + np.cos(lat1)*np.cos(lat2)*np.sin((lon2-lon1)/2)**2))
    
#     # calculate the coordinates of the points along the great circle path
#     # use the spherical law of cosines
#     lat = np.zeros(npts)
#     lon = np.zeros(npts)
#     for i in range(npts):
#         f = float(i)/float(npts-1)
#         a = np.sin((1-f)*d)/np.sin(d)
#         b = np.sin(f*d)/np.sin(d)
#         x = a*np.cos(lat1)*np.cos(lon1) + b*np.cos(lat2)*np.cos(lon2)
#         y = a*np.cos(lat1)*np.sin(lon1) + b*np.cos(lat2)*np.sin(lon2)
#         z = a*np.sin(lat1) + b*np.sin(lat2)
#         lat[i] = np.arctan2(z, np.sqrt(x**2 + y**2))
#         lon[i] = np.arctan2(y, x)
        
#     # convert back to degrees
#     lat = np.degrees(lat)
#     lon = np.degrees(lon)
    
#     return lat, lon

# # define the start and end points
# lat1 = 90
# lon1 = 0
# lat2 = 0
# lon2 = -110.0

# # calculate the great circle path
# npts = 100
# lat, lon = great_circle_path(lat1, lon1, lat2, lon2, npts)

# # plot the great circle path
# plt.figure()
# plt.plot(lon, lat, 'r-')
# plt.xlabel('Longitude (deg)')
# plt.ylabel('Latitude (deg)')
# plt.title('Great Circle Path')
# plt.show()


# # plot the great circle path on a map using cartopy
# import cartopy.crs as ccrs
# import cartopy.feature as cfeature

# # create a map projection
# proj = ccrs.LambertConformal(central_longitude=-100.0, central_latitude=40.0)

# # create a figure
# fig = plt.figure(figsize=(8,8))

# # add a map to the figure and specify the projection
# ax = fig.add_subplot(1,1,1, projection=proj)

# # add some map features
# # ax.add_feature(cfeature.LAND)
# # ax.add_feature(cfeature.OCEAN)
# # ax.add_feature(cfeature.COASTLINE)
# # ax.add_feature(cfeature.BORDERS)

# # plot the great circle path
# ax.plot(lon, lat, 'r-', transform=ccrs.Geodetic())

# # add a title
# ax.set_title('Great Circle Path')

# # add a lat/lon grid
# ax.gridlines()

# # show the figure
# plt.show()

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Define the unit vector
unit_vector = np.array([1, 1, 1])
# Normalize the vector to make it a unit vector
unit_vector = unit_vector / np.linalg.norm(unit_vector)

# Create a figure and a 3D subplot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Create spherical coordinates
u = np.linspace(0, 2 * np.pi, 100)
v = np.linspace(0, np.pi, 100)
u, v = np.meshgrid(u, v)

# Convert spherical coordinates to Cartesian coordinates
x = np.cos(u) * np.sin(v)
y = np.sin(u) * np.sin(v)
z = np.cos(v)

# print(unit_vector)
# Plot the unit vector as a line
# ax.plot([0, unit_vector[0]], [0, unit_vector[1]], [0, unit_vector[2]], color='r', lw=3)
ax.quiver(0, 0, 0, unit_vector[0], unit_vector[1], unit_vector[2], color='b', label='Unit Vector')
ax.plot(0,0,0, marker='+', color='r', label='Origin')

# Plot the unit sphere 
ax.plot_surface(x, y, z, color='b', alpha=0.2)

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


