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
            steps {
		sh 'make test'
	    }
	}
    }
}
