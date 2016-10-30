FROM python:3

RUN apt-get update && apt-get install -y python-pycurl python-pip

RUN pip install pyresttest

COPY testing_allusers.yaml /home
COPY testing_users.yaml /home
COPY testing_contacts.yaml /home
COPY testing_recommendations.yaml /home
COPY testing_messages.yaml /home

COPY entrypoint.sh /home/entrypoint.sh
ENTRYPOINT /home/entrypoint.sh
