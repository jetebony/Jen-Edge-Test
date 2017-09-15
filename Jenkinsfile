pipeline {
    agent any
    stages {
	stage('Clean') {
            steps {
                sh 'make clean'
            }
        }
        stage('Build') {
            steps {
		sh 'make arm'
            }
        }
	stage('Test') {
	    environment { 
                LD_LIBRARY_PATH = "/lib:/usr/lib"
            }
            steps {
		env
		sh 'make testarm'
            }
	}
    }
}
