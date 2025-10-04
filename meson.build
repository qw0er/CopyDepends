project('copyDepends', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++2a'])

boostD=dependency('boost',modules:['program_options'])
testD=dependency('boost',modules: 'unit_test_framework')

exe=executable('copyDepends',['main.cpp','func.cpp'],dependencies: boostD,install: true)

copyDepends = executable('copyDepends-test', ['test.cpp','func.cpp'],dependencies: [testD,boostD], install : true)

test('test', copyDepends)