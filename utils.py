import pandas as pd
import plotly.express as px

df = pd.read_csv("signaltap.csv")

print(df.info())

fig = px.scatter(df, x='t', y='c0')

fig.show()
