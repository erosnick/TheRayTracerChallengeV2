import bpy
import yaml
import mathutils

def export_spheres():
    # Retrieve all the spheres in the scene
    spheres = [obj for obj in bpy.data.objects if obj.type == 'MESH' and 'Sphere' in obj.data.name]
    planes = [obj for obj in bpy.data.objects if obj.type == 'MESH' and 'Plane' in obj.data.name]

    exportData = {
        "scene":{}
    }
    scene = exportData['scene']
    scene['camera'] = {}
    
    # Get the active camera in the scene
    camera = bpy.context.scene.camera

    # Check if a camera is set as active
    if camera is not None and camera.type == 'CAMERA':
        # Retrieve camera information
        camera_name = camera.name
        camera_location = camera.location
        camera_rotation = camera.rotation_euler
        camera_lens = camera.data.lens

        # Print the camera information
        # print("Camera Name:", camera_name)
        # print("Camera Location:", camera_location)
        # print("Camera Rotation:", camera_rotation)
        # print("Camera Lens:", camera_lens)

        # Get the camera sensor size
        sensor_width = camera.data.sensor_width
        sensor_height = camera.data.sensor_height

        print("Camera data:", camera.data)

        # Get the render resolution
        render_resolution_x = bpy.context.scene.render.resolution_x
        render_resolution_y = bpy.context.scene.render.resolution_y

        # Calculate the camera view size
        view_width = render_resolution_x * (sensor_width / sensor_height)
        view_height = render_resolution_y

         # Calculate the eye position
        eye_position = camera_location

        # Calculate the target position
        # Assuming camera is looking down the negative Z-axis
        target_direction = mathutils.Vector((0, 0, -1))
        target_direction.rotate(camera_rotation)
        target_position = camera_location + target_direction

        # Calculate the up vector
        up_direction = mathutils.Vector((0, 1, 0))
        up_direction.rotate(camera_rotation)
        up_vector = up_direction

        scene['camera']['imageWidth'] = render_resolution_x
        scene['camera']['imageHeight'] = render_resolution_y
        scene['camera']['center'] = [target_position.x, target_position.z, target_position.y, 1.0]
        scene['camera']['up'] = [up_vector.x, up_vector.z, up_vector.y, 1.0]

        # Print the camera view size
        # print("Camera View Width:", render_resolution_x)
        # print("Camera View Height:", render_resolution_y)
    else:
        print("No active camera found or the active object is not a camera.")

    # Iterate over the spheres and export their parameters

    scene['name'] = 'Test'
    scene['objects'] = {}
    scene['camera']['fov'] = camera_lens
    scene['camera']['eye'] = [eye_position.x, eye_position.z, eye_position.y, 1.0]
    scene['camera']['rotation'] = [camera_rotation.x, camera_rotation.z, camera_rotation.y, 1.0]

    objects = exportData['scene']['objects']

    for sphere in spheres:
        objects['sphere'] = {}
        objects['sphere']['type'] = 0
        objects['sphere']['radius'] = sphere.scale.x * 0.5
        objects['sphere']['center'] = sphere.scale.x * 0.5
        objects['sphere']['transform'] = {}
        objects['sphere']['center'] = [0.0, 0.0, 0.0, 1.0]
        objects['sphere']['transform']['translation'] = [sphere.location.x, sphere.location.z, sphere.location.y, 1.0]
        objects['sphere']['transform']['rotation'] = [sphere.rotation_euler.x, sphere.rotation_euler.z, sphere.rotation_euler.y, 1.0]
        objects['sphere']['transform']['scale'] = [sphere.scale.x, sphere.scale.z, sphere.scale.y, 1.0]
        objects['sphere']['material'] = {}
        objects['sphere']['material']['color'] = [1.0, 1.0, 1.0, 0.0]
        objects['sphere']['material']['ambient'] = 0.1
        objects['sphere']['material']['diffuse'] = 1.0
        objects['sphere']['material']['specular'] = 0.3
        objects['sphere']['material']['shininess'] = 200

    for plane in planes:
        objects['plane'] = {}
        objects['plane']['type'] = 1
        objects['plane']['transform'] = {}
        objects['plane']['transform']['translation'] = [plane.location.x, plane.location.z, plane.location.y, 1.0]
        objects['plane']['transform']['rotation'] = [plane.rotation_euler.x, plane.rotation_euler.z, plane.rotation_euler.y, 1.0]
        objects['plane']['transform']['scale'] = [plane.scale.x, plane.scale.z, plane.scale.y, 1.0]
        objects['plane']['extent'] = plane.scale.x * 0.5
        objects['plane']['material'] = {}
        objects['plane']['material']['color'] = [1.0, 1.0, 1.0, 0.0]
        objects['plane']['material']['ambient'] = 0.1
        objects['plane']['material']['diffuse'] = 1.0
        objects['plane']['material']['specular'] = 0.3
        objects['plane']['material']['shininess'] = 200

    with open('./BlenderScene.yaml', 'w', encoding='utf-8') as f:
        yaml.dump(data = exportData, stream = f, allow_unicode = True)

if __name__ == '__main__':
    export_spheres()