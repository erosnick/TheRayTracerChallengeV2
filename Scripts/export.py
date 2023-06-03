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
        camera_angle = camera.data.angle

        # Print the camera information
        # print("Camera Name:", camera_name)
        # print("Camera Location:", camera_location)
        # print("Camera Rotation:", camera_rotation)
        # print("Camera Lens:", camera_lens)

        # Get the camera sensor size
        sensor_width = camera.data.sensor_width
        sensor_height = camera.data.sensor_height

        print("Camera location:", camera_location)

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
        scene['camera']['up'] = [up_vector.x, up_vector.z, up_vector.y, 0.0]

        # Print the camera view size
        # print("Camera View Width:", render_resolution_x)
        # print("Camera View Height:", render_resolution_y)
    else:
        print("No active camera found or the active object is not a camera.")

    # Iterate over the spheres and export their parameters

    scene['name'] = 'Test'
    scene['objects'] = {}
    scene['camera']['fov'] = camera_angle
    scene['camera']['eye'] = [eye_position.x, eye_position.z, eye_position.y, 1.0]
    scene['camera']['rotation'] = [camera_rotation.x, camera_rotation.z, camera_rotation.y, 1.0]

    objects = exportData['scene']['objects']

    for sphere in spheres:
        objects['sphere'] = {}
        objects['sphere']['type'] = 0
        objects['sphere']['radius'] = sphere.scale.x
        objects['sphere']['center'] = sphere.scale.x * 0.5
        objects['sphere']['transform'] = {}
        objects['sphere']['center'] = [0.0, 0.0, 0.0, 1.0]
        objects['sphere']['transform']['translation'] = [sphere.location.x, sphere.location.z, sphere.location.y, 1.0]
        objects['sphere']['transform']['rotation'] = [sphere.rotation_euler.x, sphere.rotation_euler.z, sphere.rotation_euler.y, 1.0]
        objects['sphere']['transform']['scale'] = [sphere.scale.x, sphere.scale.z, sphere.scale.y, 1.0]
        objects['sphere']['material'] = {}
        objects['sphere']['material']['ambient'] = 0.1
        objects['sphere']['material']['diffuse'] = 1.0
        objects['sphere']['material']['specular'] = 0.3
        objects['sphere']['material']['shininess'] = 200

        material = sphere.active_material

        # Find the Principled BSDF node
        principled_node = None
        for node in material.node_tree.nodes:
            if node.type == 'BSDF_PRINCIPLED':
                principled_node = node
                break
        if principled_node:
            base_color = principled_node.inputs['Base Color'].default_value
            metallic = principled_node.inputs['Metallic'].default_value
            roughness = principled_node.inputs['Roughness'].default_value
            ior = principled_node.inputs['IOR'].default_value
            specular = principled_node.inputs['Specular'].default_value
            transmission = principled_node.inputs['Transmission'].default_value
            objects['sphere']['material']['color'] = [base_color[0], base_color[1], base_color[2], 0.0]
            objects['sphere']['material']['specular'] = specular
            objects['sphere']['material']['reflective'] = metallic
            objects['sphere']['material']['refractiveIndex'] = ior
            objects['sphere']['material']['transparency'] = transmission
            print("Base Color:", base_color)
            print("Metallic:", metallic)
            print("Roughness:", roughness)
            print("IOR:", ior)

    for plane in planes:
        objects['plane'] = {}
        objects['plane']['type'] = 1
        objects['plane']['transform'] = {}
        objects['plane']['transform']['translation'] = [plane.location.x, plane.location.z, plane.location.y, 1.0]
        objects['plane']['transform']['rotation'] = [plane.rotation_euler.x, plane.rotation_euler.z, plane.rotation_euler.y, 1.0]
        objects['plane']['transform']['scale'] = [plane.scale.x, plane.scale.z, plane.scale.y, 1.0]
        objects['plane']['extent'] = 1.0
        objects['plane']['material'] = {}
        objects['plane']['material']['color'] = [1.0, 1.0, 1.0, 0.0]
        objects['plane']['material']['ambient'] = 0.1
        objects['plane']['material']['diffuse'] = 1.0
        objects['plane']['material']['specular'] = 0.3
        objects['plane']['material']['shininess'] = 200

        material = plane.active_material

        principled_node = None
        for node in material.node_tree.nodes:
            if node.type == 'BSDF_PRINCIPLED':
                principled_node = node
                break
        if principled_node:
            base_color = principled_node.inputs['Base Color'].default_value
            metallic = principled_node.inputs['Metallic'].default_value
            roughness = principled_node.inputs['Roughness'].default_value
            ior = principled_node.inputs['IOR'].default_value
            specular = principled_node.inputs['Specular'].default_value
            transmission = principled_node.inputs['Transmission'].default_value
            objects['plane']['material']['color'] = [base_color[0], base_color[1], base_color[2], 0.0]
            objects['plane']['material']['specular'] = specular
            objects['plane']['material']['reflective'] = metallic
            objects['plane']['material']['refractiveIndex'] = ior
            objects['plane']['material']['transparency'] = transmission

    scene['lights'] = {}

    scene['lights']['pointLight'] = {}

    # Get the active light in the scene
    light = bpy.context.scene.objects.get("Point")

    # Check if the active object is a point light
    if light is not None and light.type == 'LIGHT' and light.data.type == 'POINT':
        # Retrieve light information
        light_name = light.name
        light_location = light.location
        light_energy = light.data.energy
        light_color = light.data.color

        # Print the light information
        print("Light Name:", light_name)
        print("Light Location:", light_location)
        print("Light Energy:", light_energy)
        print("Light Color:", light_color)
        print("Light type:", light.data.type)

        scene['lights']['pointLight']['location'] = [light_location.x, light_location.z, light_location.y, 1.0]
        scene['lights']['pointLight']['color'] = [light_color.r, light_color.g, light_color.b, 0.0]
        scene['lights']['pointLight']['light_energy'] = light_energy

    else:
        print("No active point light found or the active object is not a point light.")

    # sphereMaterial = bpy.data.materials['Sphere']

    # print("Material:", sphereMaterial)

    with open('E:/OpenSources/RayTracing/TheRayTracerChallengeV2/Assets/Scenes/BlenderScene.yaml', 'w', encoding='utf-8') as f:
        yaml.dump(data = exportData, stream = f, allow_unicode = True)

if __name__ == '__main__':
    export_spheres()