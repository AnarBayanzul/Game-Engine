import bpy, struct

# Making enum
class normalEnum:
    vertex, polygon = range(2)

bytes = False
bones = False
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
       
       
    # stuff for armature
    
    # armature data:
        # identifier conditional if bones preset
            # bone indices[4]
            # bone weights[4]
    boneCount = 0
    boneHeads= [] # parent-child separation
    boneTails = [] # bone location with respect to parent
    if bones:        
        selectedArmature.vertex_groups # array of vertex_groups created from armature
        selectedMesh.vertices # group data
        
        boneCount = len(meshObj.vertex_groups)
        print(boneCount)
        print(len(selectedMesh.vertices))
        
        print(type(selectedArmature))
        for bone in selectedArmature.data.bones:
            print(bone.tail)
            boneTails.append(bone.tail)
            boneHeads.append(bone.head)
        
        for v in selectedMesh.vertices:
            for g in v.groups:
                print(g.group, end=":  ")
                print(g.weight, end=",")
            print("")
                  
        
      
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
