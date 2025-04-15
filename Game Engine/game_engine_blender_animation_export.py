import bpy, struct

# Making enum
class normalEnum:
    vertex, polygon = range(2)

bytes = False
bones = True
normalType = normalEnum.vertex

def write_some_data(context, filepath, use_some_setting):
    # Whether writing bytes or plain text
    print("running write_some_data...")
    if bytes:
        f = open(filepath, 'wb')
    else:
        f = open(filepath, 'w', encoding='utf-8')

#    f.write("Hello World {:g}".format(use_some_setting))
    

    
    # The meaty stuff
    delimiter = ' '
    

    for obj in bpy.context.view_layer.objects:
        if type(obj.data) == bpy.types.Mesh:
            meshObj = obj
            selectedMesh = obj.data
        elif type(obj.data) == bpy.types.Armature:
            selectedArmature = obj
    
    nameIndex = {}
    for i in range(len(meshObj.vertex_groups)):
        nameIndex[meshObj.vertex_groups[i].name] = i
    
    
    

    
    tracks = selectedArmature.animation_data.nla_tracks
    keyframes = set()
    boneIDs = set()
    
    for track in tracks:
        for strip in track.strips:
            for fcu in strip.action.fcurves:
#                print(fcu.data_path + " channel " + str(fcu.array_index))
                boneName = bpy.path.basename(fcu.data_path).split("\"")[1]
                boneIDs.add((nameIndex[boneName], boneName))
                for keyframe in fcu.keyframe_points:
                    keyframes.add(keyframe.co[0])
                
    keyframes = list(keyframes)
    keyframes.sort()
    boneIDs = list(boneIDs)
    boneIDs.sort()
    
    animFPS = bpy.context.scene.render.fps


        
    if bytes:
        pass
    else:
        output = ""
        output += str(len(keyframes))
        output += str(delimiter)
        output += str(len(boneIDs))
        for frameCount in keyframes:
            #time stamp
            output += str(delimiter)
            output += str((frameCount - 1) / animFPS)
            bpy.context.scene.frame_set(int(frameCount))
            for boneID, boneName in boneIDs:
                output += str(delimiter)
                output += str(boneID)
                for value in selectedArmature.pose.bones[boneName].rotation_quaternion:
                    output += str(delimiter)
                    output += str(value)
#                print(boneName, boneID, end=":\t")
#                print(selectedArmature.pose.bones[boneName].rotation_quaternion)
 
        f.write(output)
 
      
#    if bytes:
#        # int and floats are 4 bytes
#        for i in range(0, len(boneIndices), 4):
#            for j in range(4):
#                f.write(struct.pack('<i', boneIndices[i + j]))
#            for j in range(4):
#                f.write(struct.pack('<f', boneWeights[i + j]))

#            
#        f.write(struct.pack('<i', boneCount))
#        for bone in bone_origin:
#            for element in bone:
#                f.write(struct.pack('<f', element))
#        for bone in parent_to_this:
#            for element in bone:
#                f.write(struct.pack('<f', element))
#        for parentIndex in boneParent:
#            f.write(struct.pack('<i', parentIndex))
#        
#    else:
#        output = ""  
#        output += str(triangleCount)              # remove
#        for i in range(0, len(boneIndices), 4):
#            for j in range(4):
#                output += str(delimiter)
#                output += str(boneIndices[i + j])
#            for j in range(4):
#                output += str(delimiter)
#                output += str(boneWeights[i + j])
#        
#        output += str(delimiter)
#        output += str(boneCount)
#        for bone in bone_origin:
#            for element in bone:
#                output += str(delimiter)
#                output += str(element)
#        for bone in parent_to_this:
#            for element in bone:
#                output += str(delimiter)
#                output += str(element)
#        for parentIndex in boneParent:
#            output += str(delimiter)
#            output += str(parentIndex)

#        
#        
#        f.write(output)
        
    
    f.close()

    return {'FINISHED'}

























# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportSomeData(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_test.some_data"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Some Data"





    # ExportHelper mix-in class uses this.
    if bytes:
        filename_ext = ".gex"
    else:
        filename_ext = ".txt"

    filter_glob: StringProperty(
        default="*.txt",
        options={'HIDDEN'},
        maxlen=255,  # Max internal buffer length, longer would be clamped.
    )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    use_setting: BoolProperty(
        name="Example Boolean",
        description="Example Tooltip",
        default = True,
    )

    type: EnumProperty(
        name="Example Enum",
        description="Choose between two items",
        items=(
            ('OPT_A', "First Option", "Description one"),
            ('OPT_B', "Second Option", "Description two"),
        ),
        default='OPT_A',
    )

    def execute(self, context):
        return write_some_data(context, self.filepath, self.use_setting)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportSomeData.bl_idname, text="My Exporter")


# Register and add to the "file selector" menu (required to use F3 search "Text Export Operator" for quick access).
def register():
    bpy.utils.register_class(ExportSomeData)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportSomeData)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_test.some_data('INVOKE_DEFAULT')
