import bpy
import yaml
import mathutils
import bpy
from mathutils import Matrix, Vector
import math

def createCamera(camera):
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

        cameraNode = {}

        cameraNode['imageWidth'] = render_resolution_x
        cameraNode['imageHeight'] = render_resolution_y
        cameraNode['eye'] = [camera_location.x, camera_location.z, camera_location.y, 1.0]
        cameraNode['center'] = [target_position.x, target_position.z, target_position.y, 1.0]
        cameraNode['up'] = [up_vector.x, up_vector.z, up_vector.y, 0.0]
        cameraNode['rotation'] = [camera_rotation.x, camera_rotation.z, camera_rotation.y, 0.0]
        cameraNode['fov'] = camera_angle
        # Print the camera view size
        # print("Camera View Width:", render_resolution_x)
        # print("Camera View Height:", render_resolution_y)
    else:
        print("No active camera found or the active object is not a camera.")
    return cameraNode

def createDefaultMaterial():
    materialNode = {}
    materialNode['ambient'] = 0.1
    materialNode['diffuse'] = 1.0
    materialNode['specular'] = 0.3
    materialNode['shininess'] = 200
    materialNode['color'] = [1.0, 1.0, 1.0, 0.0]
    materialNode['reflective'] = 0.0
    materialNode['refractiveIndex'] = 1.0
    materialNode['transparency'] = 0.0
    return materialNode

def createMaterial(material):
    materialNode = {}
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
        materialNode['color'] = [base_color[0], base_color[1], base_color[2], 0.0]
        materialNode['ambient'] = 0.1
        materialNode['diffuse'] = 1.0
        materialNode['specular'] = specular
        materialNode['shininess'] = 200
        materialNode['reflective'] = metallic
        materialNode['refractiveIndex'] = ior
        materialNode['transparency'] = transmission
        print("Base Color:", base_color)
        print("Metallic:", metallic)
        print("Roughness:", roughness)
        print("IOR:", ior)
    return materialNode

def createSphere(sphere):
    sphereNode = {}
    sphereNode['name'] = sphere.name
    sphereNode['type'] = 0
    sphereNode['radius'] = 1.0
    sphereNode['center'] = [0.0, 0.0, 0.0, 1.0]
    sphereNode['transform'] = {}
    sphereNode['transform']['translation'] = [sphere.location.x, sphere.location.z, sphere.location.y, 1.0]
    sphereNode['transform']['rotation'] = [sphere.rotation_euler.x, -sphere.rotation_euler.z, sphere.rotation_euler.y, 1.0]
    sphereNode['transform']['scale'] = [sphere.scale.x, sphere.scale.z, sphere.scale.y, 1.0]
    sphereNode['material'] = createDefaultMaterial()
    
    material = sphere.active_material

    if material:
        sphereNode['material'] = createMaterial(material)
    else:
        print("No material.")
    return sphereNode

def createPlane(plane):
    planeNode = {}
    planeNode['name'] = plane.name
    planeNode['type'] = 1
    planeNode['transform'] = {}
    planeNode['transform']['translation'] = [plane.location.x, plane.location.z, plane.location.y, 1.0]
    planeNode['transform']['rotation'] = [plane.rotation_euler.x, -plane.rotation_euler.z, plane.rotation_euler.y, 1.0]
    planeNode['transform']['scale'] = [plane.scale.x, plane.scale.z, plane.scale.y, 1.0]
    planeNode['extent'] = 1.0
    planeNode['material'] =  createDefaultMaterial()

    material = plane.active_material

    if material:
        planeNode['material'] = createMaterial(material)
    else:
        print("No material.")

    patternName = plane.data.get("Pattern")
    patternType = plane.data.get("PatternType")
    color1 = plane.data.get("Color1")
    color2 = plane.data.get("Color2")
    print("Color1", color1)
    if patternName:
        print("Pattern:", patternName)
        planeNode['material']['pattern'] = {}
        print("Pattern Type:", patternType)
        planeNode['material']['pattern']['type'] = patternType
        if color1:
            planeNode['material']['pattern']['color1'] = [color1[0], color1[1], color1[2], 0.0]
        if color2:
            planeNode['material']['pattern']['color2'] = [color2[0], color2[1], color2[2], 0.0]

    return planeNode

def createCube(cube):
    cubeNode = {}
    cubeNode['name'] = cube.name
    cubeNode['type'] = 2
    cubeNode['transform'] = {}
    cubeNode['transform']['translation'] = [cube.location.x, cube.location.z, cube.location.y, 1.0]
    cubeNode['transform']['rotation'] = [cube.rotation_euler.x, -cube.rotation_euler.z, cube.rotation_euler.y, 1.0]
    cubeNode['transform']['scale'] = [cube.scale.x, cube.scale.z, cube.scale.y, 1.0]
    cubeNode['material'] =  createDefaultMaterial()

    material = cube.active_material

    if material:
        cubeNode['material'] = createMaterial(material)
    else:
        print("No material")
    return cubeNode

def createPointLight(light):
    pointLightNode = {}
    pointLightNode['name'] = light.name
    # Retrieve light information
    # Print the light information
    print("Light Name:", light.name)
    print("Light Location:", light.location)
    print("Light Energy:", light.data.energy)
    print("Light Color:", light.data.color)
    print("Light type:", light.data.type)

    pointLightNode['location'] = [light.location.x, light.location.z, light.location.y, 1.0]
    pointLightNode['color'] = [light.data.color.r, light.data.color.g, light.data.color.b, 0.0]
    pointLightNode['lightEnergy'] = light.data.energy

    return pointLightNode

def exportScene():
    # Retrieve all the spheres in the scene
    spheres = [obj for obj in bpy.data.objects if obj.type == 'MESH' and 'Sphere' in obj.data.name]
    planes = [obj for obj in bpy.data.objects if obj.type == 'MESH' and 'Plane' in obj.data.name]
    cubes = [obj for obj in bpy.data.objects if obj.type == 'MESH' and 'Cube' in obj.data.name]
    
    exportData = {
        "scene":{}
    }

    scene = exportData['scene']
    scene['name'] = bpy.context.scene.name
    scene['camera'] = {}
    scene['objects'] = {}
    
    # Get the active camera in the scene
    camera = bpy.context.scene.camera

    scene['camera'] = createCamera(camera)

    # Iterate over the spheres and export their parameters
    objects = exportData['scene']['objects']

    for sphere in spheres:
        objects[sphere.name] = createSphere(sphere)

    for plane in planes:
        objects[plane.name] = {}
        objects[plane.name] = createPlane(plane)

    for cube in cubes:
        objects[cube.name] = {}
        objects[cube.name] = createCube(cube)

    scene['lights'] = {}

    pointLights = [obj for obj in bpy.data.objects if obj.type == 'LIGHT' and obj.data.type == 'POINT']

    for light in pointLights:
        # Get the active light in the scene
        # Check if the active object is a point light
        if light is not None and light.type == 'LIGHT' and light.data.type == 'POINT':
            scene['lights'][light.name] = createPointLight(light)
        else:
            print("No active point light found or the active object is not a point light.")

    # sphereMaterial = bpy.data.materials['Sphere']

    # print("Material:", sphereMaterial)

    path = 'E:/OpenSources/RayTracing/TheRayTracerChallengeV2/Assets/Scenes/'
    with open(path + bpy.context.scene.name + '.yaml', 'w', encoding='utf-8') as f:
        yaml.dump(data = exportData, stream = f, allow_unicode = True)
        print("Write to ", path)

# if __name__ == '__main__':
#     exportScene()

mode_keymap = None

class RollOperator(bpy.types.Operator):
    bl_idname = "transform.roll"
    bl_label = "Roll active object"
    def execute(self, context):
        dx = self.dx / 100
        dy = self.dy / 100
        #matrix = Matrix.Rotation((dx**2 + dy**2)**.5, 4, Vector((-dy, dx, 0)))
        #matrix *= context.object.matrix_world
        context.object.rotation_euler.x += dx * math.pi / 4
        context.object.rotation_euler.y += dy * math.pi / 4
        context.object.location.x += dx
        context.object.location.y += dy
        return {'FINISHED'}
    def modal(self, context, event):
        if event.type == 'MOUSEMOVE':  # Apply
            self.dx = event.mouse_x - event.mouse_prev_x
            self.dy = event.mouse_y - event.mouse_prev_y            
            self.execute(context)
        elif event.type == 'LEFTMOUSE':  # Confirm
            return {'FINISHED'}
        elif event.type in ('RIGHTMOUSE', 'ESC'):  # Cancel
            context.object.location = self.location
            context.object.rotation_euler = self.rotation_euler
            return {'CANCELLED'}
        return {'RUNNING_MODAL'}
    def invoke(self, context, event):
        rotation = context.object.rotation_euler
        location = context.object.location
        self.location = location[0], location[1], location[2]
        self.rotation_euler = rotation[0], rotation[1], rotation[2]        
        self.dx = 0
        self.dy = 0        
        self.execute(context)
        # context.window_manager.modal_handler_add(self)
        exportScene()
        return {'RUNNING_MODAL'}


bpy.utils.register_class(RollOperator)

key_config = bpy.context.window_manager.keyconfigs.addon
if key_config:
    key_map = key_config.keymaps.new(name='3D View', space_type='VIEW_3D')
    key_entry = key_map.keymap_items.new("transform.roll",
                                        type='Q',
                                        value='PRESS',
                                        shift=True,
    )
    mode_keymap = (key_map, key_entry)