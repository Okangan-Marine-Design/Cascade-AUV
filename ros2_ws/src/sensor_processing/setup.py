from setuptools import find_packages, setup

package_name = 'sensor_processing'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='eryk',
    maintainer_email='erykhalicki0@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'pid=sensor_processing.PIDController:main',
            'pid_combiner=sensor_processing.PIDCombiner:main',
            'sim_adapter=sensor_processing.SimAdapter:main',
            'eryk-pub=sensor_processing.ErykTestPub:main',
            'eryk-sub=sensor_processing.ErykTestSub:main'
        ],
    },
)
