import RaisedButton  from 'material-ui/RaisedButton'
import TextField     from 'material-ui/TextField'
import { Card, 
         CardText,
         CardTitle } from 'material-ui/Card'
import * as React    from 'react'

const FormLogin = ({
  onSubmit,
  onChange,
  errors,
  user,
}) => (
    <form>
        <Card className="container">
            <CardTitle title="AgencyDB tracker" subtitle="login page" />

            <div className="field-line">
                <TextField
                floatingLabelText="Name"
                name="name"
                errorText={errors.name}
                onChange={onChange}
                value={user.name}
                />
            </div>

            <div className="field-line">
                <TextField
                floatingLabelText="Email"
                name="email"
                errorText={errors.email}
                onChange={onChange}
                value={user.email}
                />
            </div>

            <div className="field-line">
                <TextField
                floatingLabelText="Password"
                type="password"
                name="password"
                onChange={onChange}
                errorText={errors.password}
                value={user.password}
                />
            </div>

            <div className="button-line">
                <RaisedButton type="submit" label="Create New Account" primary />
            </div>

            <CardText>Already have an account?</CardText>

        </Card>
    </form>
);

export default FormLogin;
