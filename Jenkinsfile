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
	    withEnv(["LD_LIBRARY_PATH=/lib:/usr/lib"]) {
		sh 'make armtest' }
	}
    }
}
