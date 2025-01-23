import bpy


def write_some_data(context, filepath, use_some_setting):
    # Whether writing bytes or plain text
    bytes = False
    print("running write_some_data...")
    if bytes:
        f = open(filepath, 'wb')
    else:
        f = open(filepath, 'w', encoding='utf-8')

#    f.write("Hello World {:g}".format(use_some_setting))
    

    
    # The meaty stuff
    delimiter = ' '
    
    selected = bpy.context.view_layer.objects.active.data
    # has polygons and vertices attributes
    
    triangleCount = len(selected.polygons)
    
    # Each vertex position per polygon
    vertices = []
    # polygon normal, three times, per polygon
    normals = []
    
    for polygon in selected.polygons:
        for index in polygon.vertices:
            vertices.append(selected.vertices[index].co)
            normals.append(polygon.normal)
            
      
    if bytes:
        f.write(triangleCount)
        for vertex in vertices:
            for element in vertex:
                f.write(delimiter)
                f.write(element)
        for normal in normals:
            for element in normal:
                f.write(delimiter)
                f.write(element)
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
        default=True,
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
