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
            'dvl=sensor_processing.Dvl:main',
            'pid=sensor_processing.PIDController:main',
            'pid_combiner=sensor_processing.PIDCombiner:main',
            'imu=sensor_processing.Imu:main',
            'Raphtestpub=sensor_processing.Raphtestpub:main',
            'Raphtestsub=sensor_processing.Raphtestsub:main'
        ],
    },
)
