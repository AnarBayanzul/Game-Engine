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
        
    
    
#    selectedMesh = bpy.context.view_layer.objects.active.data # this must be type bpy_types.Mesh
#    selectedArmature = None
#    if type(selectedMesh) == bpy.types.Armature:
#        for obj in bpy.data.objects:
#            if obj.parent == selectedMesh:
#                selectedArmature = selectedMesh
#                selectedMesh = obj
    
    
    # has polygons and vertices attributes
    
    triangleCount = len(selectedMesh.polygons)
    
    # Each vertex position per polygon
    vertices = []
    # polygon normal, three times, per polygon
    normals = []

    uvs = []
    
        # each entry in lists below represent one vertex
    boneIndices = []
    boneWeights = []

    times = 0
    for polygon in selectedMesh.polygons:
        for index in polygon.vertices:
            vertices.append(selectedMesh.vertices[index].co)
            if normalType == normalEnum.polygon:
                normals.append(polygon.normal)
            elif normalType == normalEnum.vertex:
                normals.append(selectedMesh.vertices[index].normal)
            # TODO this is super inefficient
            for vert_idx, loop_idx in zip(polygon.vertices, polygon.loop_indices):
                uv_coords = selectedMesh.uv_layers.active.data[loop_idx].uv
                if vert_idx == index:
                    uvs.append(uv_coords)
                    break
            # bone weights
            if bones:
                groupCount = 0
                for g in selectedMesh.vertices[index].groups:
                    boneIndices.append(g.group)
                    boneWeights.append(g.weight)
                    groupCount += 1
                while groupCount < 4:
                    boneIndices.append(0)
                    boneWeights.append(0.0)
                    groupCount += 1
       
    # stuff for armature
    if bones:    
        nameIndex = {}
        for i in range(len(meshObj.vertex_groups)):
            nameIndex[meshObj.vertex_groups[i].name] = i
        # armature data:
            # identifier conditional if bones preset
                # bone indices[4]
                # bone weights[4]

        boneCount = len(meshObj.vertex_groups)
        parent_to_this = [None] * boneCount # vector between head of parent and head of child
        bone_origin = [None] * boneCount # original head position of a bone (local space of mesh
        boneParent = [-1] * boneCount # index of parent
    
        selectedArmature.vertex_groups # array of vertex_groups created from armature
        selectedMesh.vertices # group data
        
        for bone in selectedArmature.data.bones:
            thisIndex = nameIndex[bone.name]
            if bone.parent == None:
                parentIndex = -1
            else:
                parentIndex = nameIndex[bone.parent.name]
            bone_origin[thisIndex] = bone.head_local                     # change to bone.head if you want parent space
            boneParent[thisIndex] = parentIndex
            if bone.parent == None:
                parent_to_this[thisIndex] = bone.head_local
            else:
                parent_to_this[thisIndex] = bone.head_local - bone.parent.head_local

#        print(boneCount) # int
#        print(len(bone_origin)) # vectors
#        print(len(parent_to_this)) # vectors
#        print(len(boneParent)) # int

    
 
      
    if bytes:
        # int and floats are 4 bytes
        f.write(struct.pack('<i', triangleCount))
        for vertex in vertices:
            for element in vertex:
#                f.write(delimiter)
                f.write(struct.pack('<f', element))
        for normal in normals:
            for element in normal:
#                f.write(delimiter)
                f.write(struct.pack('<f', element))
        for uv in uvs:
            for element in uv:
                f.write(struct.pack('<f', element))
        for i in range(0, len(boneIndices), 4):
            for j in range(4):
                f.write(struct.pack('<i', boneIndices[i + j]))
            for j in range(4):
                f.write(struct.pack('<f', boneWeights[i + j]))

            
        f.write(struct.pack('<i', boneCount))
        for bone in bone_origin:
            for element in bone:
                f.write(struct.pack('<f', element))
        for bone in parent_to_this:
            for element in bone:
                f.write(struct.pack('<f', element))
        for parentIndex in boneParent:
            f.write(struct.pack('<i', parentIndex))
        
    else:
        output = ""  
        output += str(triangleCount)
        for vertex in vertices:
            for element in vertex:
                output += str(delimiter)
                output += str(element)
        for normal in normals:
            for element in normal:
                output += str(delimiter)
                output += str(element)
        for uv in uvs:
            for element in uv:
                output += str(delimiter)
                output += str(element)
        for i in range(0, len(boneIndices), 4):
            for j in range(4):
                output += str(delimiter)
                output += str(boneIndices[i + j])
            for j in range(4):
                output += str(delimiter)
                output += str(boneWeights[i + j])
        
        output += str(delimiter)
        output += str(boneCount)
        for bone in bone_origin:
            for element in bone:
                output += str(delimiter)
                output += str(element)
        for bone in parent_to_this:
            for element in bone:
                output += str(delimiter)
                output += str(element)
        for parentIndex in boneParent:
            output += str(delimiter)
            output += str(parentIndex)

        
        
        f.write(output)
        
    
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
