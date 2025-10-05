project('copy-depends', 'cpp',
        version : '1.1.1',
        default_options : ['warning_level=3', 'cpp_std=c++23'])

boostD=dependency('boost',modules:['program_options'])
testD=dependency('boost',modules: 'unit_test_framework')

#config=configuration_data()
#
#config.set_quoted('EXCLUDELIST_PATH',join_paths(get_option('prefix'),get_option('datadir'),meson.project_name(),'excludelist.txt'))
#configFile=configure_file(output: 'cdmicro.hpp',configuration: config)

exe=executable('copy-depends',['main.cpp','func.cpp'],dependencies: boostD,install: true)

copyDepends = executable('copyDepends-test', ['test.cpp','func.cpp'],dependencies: [testD,boostD])
test('test', copyDepends)

install_data(join_paths(get_option('datadir'),meson.project_name(),'excludelist.txt'),rename: 'excludelist.txt')

py=find_program('python')
meson.add_install_script(py,'../fpmPackage.py')