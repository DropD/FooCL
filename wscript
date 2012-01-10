# !/usr/bin/env python
# encoding: utf8

APPNAME = 'fcl_test'
VERSION = 'alpha'

out = 'build'

ocl_inc_osx = '-framework OpenCL -D __CL_ENABLE_EXCEPTIONS'

olevel = 2

def options(ctx):
    ctx.load('compiler_cxx')

def configure(ctx):
    ctx.load('compiler_cxx')

    env = ctx.env

    if env.DEST_OS == 'darwin':
        env.append_value('OCL_FLAGS', ['{0}'.format(ocl_inc_osx)])
    elif env.DEST_OSX == 'linux':
        env.append_value('OCL_FLAGS', ['{0}'.format(ocl_inc_linux)])

    env.append_value('OFLAGS', ['-O{0}'.format(olevel)])

def build(ctx):
    test = ctx(source       = 'test.cpp',
               target       = 'test',
               deps         = 'parser.hpp FooCL.hpp',
               install_path = './',
               rule         = '${CXX} ${INC_FLAGS} ${SRC} -o ${TGT} ${OCL_FLAGS} && ./${TGT}')

    #mxp  = ctx(source       = 'mini_xml1.cpp',
    #           target       = 'mini_xml1',
    #           rule         = '${CXX} ${INC_FLAGS} ${SRC} -o ${TGT}')

    mxp = ctx.program(source  = 'mini_xml1.cpp',
                      target  = 'mini_xml1',
                      include = '. /usr/local')

    #run = ctx(source = 'mini_xml1',
    #          rule   = './${SRC}')

def install(ctx):
    test = ctx(source       = 'test',
               rule         = 'chmod 755 ${SRC}')

