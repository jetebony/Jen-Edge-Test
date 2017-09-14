pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'make clean'
		sh 'make arm'
            }
        }
	stage('Test)' {
            steps {
		sh 'make test'
	    }
	}
    }
}
