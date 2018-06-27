import csv
import plotly
import plotly.graph_objs as go
from plotly.offline import init_notebook_mode, plot
import base64

# init_notebook_mode(connected=True)

# plotly.tools.set_credentials_file(username='', api_key='')

import base64
with open("./nba-plan3.png", "rb") as image_file:
    encoded_string = base64.b64encode(image_file.read()).decode()
# add the prefix that plotly will want when using the string as source
encoded_image = "data:image/png;base64," + encoded_string

print('1. start to parse data.')
with open('testdata.csv', 'r', encoding='UTF-8') as csv_file:
    reader = csv.reader(csv_file)
    old_rows = [row for row in reader]

'''
shot_trace = go.Scatter(
    x=[row[2] for row in old_rows],
    y=[row[3] for row in old_rows],
    mode='markers'
)

'''

x = []
y = []
d = []
for row in old_rows:
    if row[1] == '0':
        x.append(row[2])
        y.append(row[3])
        d.append(row[4])

missed_shot_trace = go.Scatter(
    x=x,
    y=y,
    hovertext=d,
    hoverinfo='text',
    mode='markers',
    name='Missed Shot',
    marker=dict(
        symbol='x',
        size=8,
        color='rgba(255, 0, 0, .8)',
        line=dict(
            width=1,
            color='rgb(0, 0, 0, 1)'
        )
    )
)

x = []
y = []
d = []
for row in old_rows:
    if row[1] == '1':
        x.append(row[2])
        y.append(row[3])
        d.append(row[4])


made_shot_trace = go.Scatter(
    x=x,
    y=y,
    hovertext=d,
    hoverinfo='text',
    mode='markers',
    name='Made Shot',
    marker=dict(
        size=8,
        color='rgba(0, 200, 100, .8)',
        line=dict(
            width=1,
            color='rgb(0, 0, 0, 1)'
        )
    )
)

data = [missed_shot_trace, made_shot_trace]
layout = go.Layout(
    images=[dict(
          source=encoded_image,
          xref="paper",
          yref="paper",
          xanchor='left',
          yanchor='bottom',
          sizex=1,
          sizey=1,
          sizing="stretch",
          opacity=0.5,
          layer="below")],
    xaxis=dict(
        visible=False,
        range=[-1500, 1500],
        fixedrange=True,
                ),
    yaxis=dict(
        visible=False,
        range=[-800, 800],
        fixedrange=True,
                ),
    title='Shots by Michael Jordan in NBA2K11 shootout session',
    showlegend=True,
    height=873,
    width=1602
    # scene=dict(aspectmode="data")
)

fig = go.Figure(data=data, layout=layout)
plot(fig)
