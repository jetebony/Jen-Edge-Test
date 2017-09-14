pipeline {
    agent any
    stages {
        stage('build') {
            steps {
		echo "Building arm version...."
                sh 'make clean'
		sh 'make arm'
            }
        }
	stage('test)' {
            steps {
		echo "Testing...."
		sh 'make test'
	    }
	}
    }
}
